# A\* Optimization Notes (`925-astar-optimized`)

Lecture-oriented summary of the changes made to `AStarGraph::GetPath`
(`api/include/ai/a_star_graph.h`, `api/src/ai/a_star_graph.cc`).
Each section is written so it can be dropped into the slides / adjusted later.

---

## 1. Closed set: `O(N)` linear scan → `O(1)` hash lookup

**Before** — the "visited" set was a `std::vector<AStarVertex>`, so every membership
test was a linear scan:

```cpp
auto visited = std::ranges::find_if(visited_vertices,
    [v](AStarVertex vCheck){ return vCheck.position == v.position; });
if (visited != visited_vertices.end()) { ... }
```

This ran **twice** per expansion (once on pop to reject duplicates, once per
neighbour). With `N` settled nodes that is `O(N)` per check and roughly
**`O(N²)`** over a whole search.

**After** — membership is a hash lookup on an `unordered_map`:

```cpp
if (came_from_.contains(v.position)) { continue; }      // pop dedup
...
if (!came_from_.contains(new_position) && walkables_.contains(new_position)) { ... }
```

Average **`O(1)`** per check; the search is now dominated by the priority queue
(`O(N log N)`).

> Teaching hook: this is the payoff of the `Vec2iHash` slide — a good hash on
> `sf::Vector2i` turns the closed set from a list scan into a constant-time probe.

---

## 2. One container for two jobs: `came_from` as closed set **and** parent chain

The old code kept **two** coupled pieces of state:

- `visited_vertices` (the storage), and
- `parent_idx` — an **index into that vector** used to walk back the path.

That coupling is why the closed set *had* to be a vector (you needed stable
indices to reconstruct the path). Replacing it required separating the two
responsibilities.

**Solution (Option B / the "came_from" map):** a single map does both jobs.

```cpp
mutable std::unordered_map<sf::Vector2i, sf::Vector2i, Vec2iHash> came_from_;
//                         ^ key = settled node      ^ value = parent it was reached from
```

- **Membership** = "is this key present?" → the closed set.
- **Parent link** = the stored value → the reverse chain.

`AStarVertex` now carries `parent_position` (the parent it was reached from)
instead of `parent_idx`. On settle we record the link; reconstruction walks the
map from the goal back to the start:

```cpp
came_from_.emplace(v.position, v.parent_position);   // settle
...
for (sf::Vector2i cur = rounded_end; cur != rounded_start; cur = came_from_.at(cur))
    path.push_back(cur);
std::ranges::reverse(path);
```

The start tile is excluded from the returned path (the NPC already stands on it).

> Why `parent_position` is **not** redundant with the map: in this lazy-deletion
> design the map is written **once, at settle time**. Several rival entries for the
> same cell (different parents, different `g`) can sit in the open queue at once,
> so the parent has to ride on the queue entry until we know which one wins the pop.

---

## 3. Reused scratch buffer → no per-call allocation (`#1`)

`came_from_` is a **member**, not a local. Each call does:

```cpp
came_from_.clear();   // keeps capacity
```

`clear()` empties the map without freeing its bucket array, so after the first
search a steady stream of NPC queries allocates **nothing** for the closed set.
This matters because `GetPath` is called frequently (once per NPC re-path).

Trade-off accepted: one shared `came_from_` per graph means `GetPath` is **not**
safe to call concurrently on the same `AStarGraph` (multi-threaded pathfinding
is not a goal at this stage). If that changes later: one graph per worker,
external locking, or a `thread_local` buffer.

---

## 4. `const` correctness

`GetPath` no longer mutates logical graph state, only the scratch buffer, so it is
marked `const` (+ `[[nodiscard]]`). The scratch member is `mutable` so the const
method may clear/fill it — the standard "mutable cache" pattern.

```cpp
[[nodiscard]] std::vector<sf::Vector2i> GetPath(sf::Vector2i start, sf::Vector2i end) const;
```

---

## 5. Correctness fixes folded in along the way

These are **bug fixes**, not just speedups, and are worth a slide of their own:

| Fix | Before | After | Effect |
|-----|--------|-------|--------|
| Goal test on rounded coords | `v.position == end` | `v.position == rounded_end` | If `end` was not grid-aligned the goal was **never** matched → search exhausted and returned no path. |
| Heuristic on rounded coords | `ManhattanDistance(start, end)` | `ManhattanDistance(rounded_start, rounded_end)` | Heuristic now measured in the same (grid-aligned) space the search runs in. |
| Edge cost scale | `v.g + 1` | `v.g + world_offset_.x` | `g` was incremented by `1` while `h` was in **world units** (e.g. 32/tile). `h` dominated `g`, degrading A\* toward greedy best-first. Now `g` and `h` share units → proper `f = g + h` balance. |

Because the grid is 4-connected with uniform cost and Manhattan distance is a
**consistent** heuristic, settling a node on its first pop is optimal — no
re-opening needed, so the closed-set design is correct.

---

## 6. Dead code removed

Dropped now that the index-based closed set is gone:

- `AStarVertex::operator==` (only used by the old set)
- nested `AStarVertex::AStarVertexHash` (only used by the old set)
- `AStarVertex::parent_idx` (replaced by `parent_position`)
- `visited_vertices` member + its constructor `reserve`

---

## Complexity summary

| | Before | After |
|---|--------|-------|
| Closed-set membership | `O(N)` (linear scan) | `O(1)` average (hash) |
| Whole search | ~`O(N²)` | `O(N log N)` (heap-bound) |
| Allocation per call | fresh `vector` every call | reused member, `clear()` keeps capacity |

---

## Possible next steps (for later lectures)

Considered and intentionally **deferred** — discussed but not applied:

1. **`g_score` relaxation variant.** Not a win on a uniform-cost grid (it trades
   duplicate queue entries for an extra hash map — roughly a wash). Becomes
   *mandatory* the moment edges get weights (roads cheaper than grass, diagonals).
2. **Flat grid array instead of hash containers.** Index `walkables_`/`came_from_`
   by `(x/offset) + (y/offset)*width` in a `std::vector`. Kills hashing and gives
   cache locality — typically the single biggest grid-A\* speedup (often 2–5×).
3. **8-byte heap entries.** Push `(f, index)` pairs instead of the full ~24-byte
   `AStarVertex`. Reviewed and rolled back here to keep the teaching code readable
   (a single `AStarVertex` + `std::priority_queue` reads top-to-bottom).
