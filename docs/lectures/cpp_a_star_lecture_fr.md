---
theme: white
css:
- _templates/css/sae_styles.css
slideNumber: true
transition: slide
---

# A\* — Pathfinding en C++ moderne
<!-- .slide: data-background="_images/01_slide_fond_GP_22_08_22.jpg" -->

<small style="color:#fff;">Cours · théorie + lecture de code + optimisations</small>

<small style="color:#fff;">Codebase de référence : `core::ai::pathfinding::FindPath` (branche <code>naive</code>)</small>

Note:
On part de la théorie A*, puis on lit *ligne par ligne* l'implémentation
réelle du projet CityBuilder, et on finit par les pistes d'optimisation.
Les étudiants travaillent sur la branche 925 (pas encore d'A*) : ce cours
leur donne la cible à atteindre.

---
<!-- .slide: data-background="_images/01_slide_fond_GP_22_08_22.jpg" -->
## Déroulé

<div style="color:#fff;">

- **Partie 1** — Qu'est-ce que A\* ? *(théorie)*
- **Partie 2** — Analyse de l'implémentation `FindPath` *(lecture de code)*
  - 2.1 Que fait chaque partie du code
  - 2.2 Comment extraire les infos de la tilemap
  - 2.3 Tricks & features : Manhattan, `std::expected`, `mdspan`, « arena », templates
  - 2.4 Quelles optimisations sur cette implé
- **Partie 3** — Optimisations possibles *(roadmap)*

</div>

---

## Partie 1
<!-- .slide: data-background="_images/01_slide_fond_GP_22_08_22.jpg" -->
### Qu'est-ce que A\* ?

---

### Le problème

- Trouver le **plus court chemin** entre deux cellules d'une grille.
- **Nœuds** = cellules ; **arêtes** = déplacements vers un voisin *walkable*.
- Une grille = un graphe implicite : pas besoin de stocker les arêtes, on les
  *calcule* (les voisins) à la volée.

→ A\* = Dijkstra **guidé** par une estimation de la distance restante.

---

### Le cœur : `f = g + h`

- **g** = coût **réel** depuis le départ jusqu'à la cellule.
- **h** = **heuristique** : estimation du coût restant jusqu'au but.
- **f = g + h** = coût total estimé d'un chemin passant par cette cellule.

A\* explore **toujours en premier** la cellule de plus petit `f`.

---

### Deux ensembles

- **Open set** (*frontière*) : cellules découvertes, pas encore traitées.
  - Implémenté par une **file de priorité** (min-`f`).
- **Closed / visited set** : cellules déjà traitées (on n'y revient pas).

Boucle : sortir le min-`f` de l'open set → si c'est le but, fini → sinon le
marquer *visited* et **détendre** ses voisins.

---

### Heuristique : admissible & cohérente

- **Admissible** : `h` ne **surestime jamais** le coût réel restant.
  - ⇒ A\* renvoie le chemin **optimal**.
- **Cohérente (monotone)** : `h(a) ≤ coût(a,b) + h(b)`.
  - ⇒ une cellule n'est jamais ré-ouverte → un simple *visited* suffit.

`h = 0` ⇒ admissible mais inutile : A\* **dégénère en Dijkstra**.

---

### A\* entre Dijkstra et glouton

| Algo | Utilise `g` | Utilise `h` | Optimal ? |
|------|:-----------:|:-----------:|:---------:|
| **Dijkstra** | ✅ | ❌ (h=0) | ✅ mais explore large |
| **Glouton (best-first)** | ❌ | ✅ | ❌ rapide mais peut rater |
| **A\*** | ✅ | ✅ | ✅ si `h` admissible |

A\* = le bon compromis : optimal **et** dirigé vers le but.

---

### Connectivité 4 vs 8

- **4-connexe** (N/S/E/O) → heuristique **Manhattan** `|dx| + |dy|`.
- **8-connexe** (+ diagonales) → **octile** / **Chebyshev**.
- ⚠️ Manhattan sur une grille 8-connexe **surestime** ⇒ plus admissible !

L'heuristique doit **coller au modèle de déplacement**.

---
<!-- .slide: data-background-iframe="_widgets/astar_widgets.html" data-background-interactive -->
### Démo interactive

Note:
Slide en plein écran sur le widget `_widgets/astar_widgets.html`.
Dessiner des murs, placer A/B, lancer en pas-à-pas. Montrer :
- l'expansion de la frontière (open) vs visited,
- les valeurs g/h/f par cellule,
- le mode Dijkstra (h=0) qui explore beaucoup plus large,
- l'effet du toggle diagonale sur le chemin.
Fallback si l'iframe ne charge pas : ouvrir _widgets/astar_widgets.html.

---

## Partie 2
<!-- .slide: data-background="_images/01_slide_fond_GP_22_08_22.jpg" -->
### Analyse de `FindPath`

<small style="color:#fff;">Fichier : <code>core/include/ai/a_star.h</code> · namespace <code>core::ai::pathfinding</code></small>

---

### Vue d'ensemble

```cpp
template <typename tile_t, typename extents_t>
std::expected<std::vector<maths::Vec2i>, PathError> FindPath(
    std::mdspan<tile_t, extents_t> tilemap,
    maths::Vec2i start_pos, maths::Vec2i end_pos);
```

- **Entrée** : une *vue* `mdspan` sur la grille + départ/arrivée (en cellules).
- **Sortie** : un chemin **ou** une erreur (`std::expected`).
- **Générique** : marche sur n'importe quelle grille de tuiles *walkable*.

---

## 2.1 — Que fait chaque partie du code

---

### Validations → `std::expected`

```cpp
if (start_pos.x < 0 || start_pos.x >= width || /* ... */)
  return std::unexpected(PathError::kStartOutOfBounds);
// ... kEndOutOfBounds ...
if (!tilemap[start_pos.x, start_pos.y].IsWalkable())
  return std::unexpected(PathError::kStartNotWalkable);
if (!tilemap[end_pos.x, end_pos.y].IsWalkable())
  return std::unexpected(PathError::kEndNotWalkable);
```

- 4 garde-fous **avant** de chercher : bornes + cases *walkable*.
- Chaque échec a un **code distinct** → l'appelant sait *pourquoi*.

---

### Les structures de données

```cpp
static constexpr std::array neighbor_dirs{ {-1,0},{0,1},{1,0},{0,-1} };
auto to_index   = [height](Vec2i p){ return p.x*height + p.y; };
auto manhattan  = [](Vec2i a, Vec2i b){ return |a.x-b.x| + |a.y-b.y|; };

struct Node { int f; Vec2i pos;
  bool operator<(const Node& o) const { return f > o.f; } }; // min-heap !

std::vector<int8_t> visited(total, 0);
std::vector<size_t> came_from(total, SIZE_MAX);
std::vector<int>    g_cost(total, INT_MAX);
std::priority_queue<Node> open_list;
```

- `operator<` **inversé** : `priority_queue` est un *max*-heap → on en fait un *min*-`f`.
- 3 tableaux **plats** indexés par cellule (`to_index`) : O(1) en lecture/écriture.

---

### La boucle principale

```cpp
while (!open_list.empty()) {
  const auto current = open_list.top(); open_list.pop();
  if (current.pos == end_pos) { /* reconstruction → return path */ }

  const auto cur_idx = to_index(current.pos);
  if (visited[cur_idx]) continue;          // déjà traité
  visited[cur_idx] = 1;

  for (const auto& dir : neighbor_dirs) {  // 4 voisins
    const auto n = current.pos + dir;
    if (/* hors grille */ || visited[n_idx] || !tilemap[n.x,n.y].IsWalkable())
      continue;
    const int tentative_g = g_cost[cur_idx] + 1;   // arête = 1
    if (tentative_g < g_cost[n_idx]) {             // détente (relaxation)
      g_cost[n_idx]  = tentative_g;
      came_from[n_idx] = cur_idx;
      open_list.push({tentative_g + manhattan(n, end_pos), n});
    }
  }
}
return std::unexpected(PathError::kNoPathFound);
```

---

### La reconstruction du chemin

```cpp
if (current.pos == end_pos) {
  std::vector<maths::Vec2i> path;
  maths::Vec2i trace = end_pos;
  while (trace != start_pos) {
    path.push_back(trace);
    const auto idx = came_from[to_index(trace)];
    trace = { idx / height, idx % height };   // index plat → (x, y)
  }
  path.push_back(start_pos);
  std::ranges::reverse(path);                 // but→départ → départ→but
  return path;
}
```

- `came_from` mémorise « d'où je viens » → on **remonte** la chaîne.
- On reconstruit à l'envers, puis on **inverse**.

---

## 2.2 — Extraire les infos de la tilemap

<small>Fichiers : <code>api/include/graphics/tilemap.h</code> · <code>api/src/motion/a_star.cc</code></small>

---

### Le contrat minimal : `WalkableCell`

```cpp
struct WalkableCell {
  Tile tile = Tile::kEmpty;
  api::graphics::SpriteRect rect{};
  [[nodiscard]] constexpr bool IsWalkable() const noexcept {
    return TileMap::IsWalkable(tile);
  }
};
```

- Le core ne demande **qu'une chose** à une cellule : `.IsWalkable()`.
- Tout le reste (sprite, type de biome…) lui est **invisible**.

---

### Exposer la grille sans copie : `AsMdspan()`

```cpp
std::mdspan<const WalkableCell, std::dextents<std::size_t, 2>>
AsMdspan() const {
  return std::mdspan(tiles_.data(), tile_count_x_, tile_count_y_);
}
```

- `tiles_` est un `std::vector<WalkableCell>` **plat**, rangé en x-major :
  `index = x * tile_count_y_ + y` (cohérent *layout-right*).
- `AsMdspan()` rend une **vue** dessus → **aucune copie** de la grille.

---

### Le pont écran ↔ grille : `GetPath`

```cpp
std::expected<Path, PathError> GetPath(const TileMap& t,
                                       sf::Vector2f start, sf::Vector2f end) {
  const int step = TileMap::step();                 // 64 px / tuile
  auto grid_path = FindPath(t.AsMdspan(),
                            ScreenToGrid(start, step),
                            ScreenToGrid(end, step));
  if (!grid_path) return std::unexpected(grid_path.error());
  // grille → pixels → Path (waypoints suivis par le Motor du NPC)
}
```

- **Séparation des responsabilités** : `FindPath` = grille pure ;
  l'API ne fait que **convertir** pixels↔cellules et **propager** l'erreur.

---

## 2.3 — Tricks & features

<small>Ce que chaque choix apporte *dans ce cas précis*.</small>

---

### 1. Manhattan vs Euclidienne

```cpp
auto manhattan = [](Vec2i a, Vec2i b){ return |a.x-b.x| + |a.y-b.y|; };
```

- Grille **4-connexe** ⇒ Manhattan = distance **exacte** sans obstacle :
  heuristique la **mieux informée**, **entière**, **sans `sqrt`**.
- Euclidienne (`√(dx²+dy²)`) reste admissible mais **sous-estime** ⇒
  A\* explore **plus de nœuds** pour le même résultat.
- Diagonales un jour ? → passer à **octile**, pas Euclidienne.

---

### 2. `std::expected<vector, PathError>`

```cpp
auto r = GetPath(tilemap, a, b);
if (r)  follow(*r);              // succès → le chemin
else switch (r.error()) {        // échec → la raison, typée
  case PathError::kNoPathFound:    /* ... */
  case PathError::kStartNotWalkable: /* ... */
}
```

- « **Valeur OU erreur** » dans un seul type, **sans exception**.
- Bien mieux qu'un `vector` vide : on sait *pourquoi* ça a échoué.
- Cousin de `std::optional`, mais qui **transporte la raison** (≈ `Result` de Rust).

---

### 3. `std::mdspan` — *quoi / comment / pourquoi*

- **Quoi** (C++23) : une **vue** multidimensionnelle, **non-propriétaire**,
  posée sur un buffer **1D contigu**. Un `span` à N dimensions. Zéro copie.
- **Comment** :
  - construite depuis un pointeur + des *extents* : `std::mdspan(data, nx, ny)` ;
  - indexée `tilemap[x, y]` (subscript multi-args C++23) ;
  - `extent(0)`/`extent(1)` = dimensions ; *layout-right* par défaut ⇒
    dernière dimension la plus rapide → index plat `x*height + y` ;
  - `dextents<size_t,2>` = dimensions connues **à l'exécution**.

---

### 3. `std::mdspan` — pourquoi ici

- **Découple** l'algo du conteneur réel : `vector`, `array`, buffer GPU…
  l'algo ne voit qu'« une grille 2D indexable ».
- **Zéro copie / zéro coût** d'abstraction.
- C'est ce qui rend le **template** naturel (`extents_t` varie).
- Contraste avec l'ancienne API qui **recopiait** un
  `std::vector<sf::Vector2f> walkableTiles` à chaque appel.

---

### 4. Pattern « arena » ? — pas vraiment

```cpp
std::vector<int8_t> visited(total, 0);
std::vector<size_t> came_from(total, SIZE_MAX);
std::vector<int>    g_cost(total, INT_MAX);
```

- Ce sont des **tableaux plats pré-alloués**, indexés par cellule.
- Ils évitent le `new`/`delete` **par nœud** (l'ancienne version *fuyait* !),
  sont **contigus** (cache-friendly) et libérés **en bloc**.
- Mais une **vraie arène** *réutiliserait* / *bump-allouerait* ces buffers ;
  ici on les **réalloue à chaque appel** → piste d'optim (Partie 3).

→ Bénéfice « façon arène », sans en être une au sens strict.

---

### 5. Pourquoi un template ici ?

```cpp
template <typename tile_t, typename extents_t>
... FindPath(std::mdspan<tile_t, extents_t> tilemap, ...);
```

- Générique sur `tile_t` : **tout** type offrant `.IsWalkable()`
  (le test utilise un simple `struct Tile { bool is_walkable; };`).
- Générique sur `extents_t` : taille/layout du `mdspan`.
- **Header-only & inliné**, **découplé** de `api::TileMap`.
- Abstraction **zéro coût** : pas de `virtual`, tout résolu **à la compilation**.

---

### Et les tests le prouvent

```cpp
TEST(Pathfinding, NonSquareGrid) {            // 4 de large × 3 de haut
  auto path = FindPath(std::mdspan(data, 4, 3), {0,0}, {3,2});
  EXPECT_EQ(path->size(), 6);                  // Manhattan = 5 → 6 cellules
}
```

- 7 tests : bornes, *not walkable*, *no path*, **grille non carrée**.
- `NonSquareGrid` verrouille l'indexation `x*height + y` (et non `x*width`).

---

### 2.4 — Optims sur CETTE implé *(pont)*

- **Réutiliser** les buffers `visited/came_from/g_cost` entre appels.
- **Partager** le graphe entre tous les NPC.
- **Tie-breaking** sur `h` pour réduire l'exploration.
- **8-connexité** + octile si on veut les diagonales.
- File de priorité plus légère (*bucket queue*) sur petits coûts entiers.

---

## Partie 3
<!-- .slide: data-background="_images/01_slide_fond_GP_22_08_22.jpg" -->
### Optimisations possibles

<small style="color:#fff;">Classées par effort / gain.</small>

---

### Réutilisation mémoire (la vraie arène)

- Aujourd'hui : `visited/came_from/g_cost` **réalloués à chaque `FindPath`**.
- Optim : des **buffers persistants** (arène / pool), juste *réinitialisés*
  entre deux recherches.
- Encore mieux : `clear()` ciblé des seules cellules touchées (ou *versioning*
  avec un compteur de génération) au lieu de tout remettre à zéro.

---

### Mutualisation — la nuance `mdspan`

- ⚠️ Le **graphe est déjà partagé sans copie** grâce au `mdspan` :
  `FindPath` travaille sur une **vue**.
- Donc l'optim **n'est PAS** « éviter de recopier la grille » — c'est **acquis**.
- Ce qui n'est **pas** mutualisé : les **buffers de recherche par appel**.
- → un seul jeu de buffers **réutilisé** par N NPC qui cherchent à tour de rôle.

---

### La direction `npc_refactoring`

- `NPCFactory` + `TileMap<TTile>` générique = l'**échafaudage** qui rend ce
  *pooling* possible **à l'échelle** (beaucoup de NPC, un graphe, des buffers
  partagés).
- L'architecture d'abord, la micro-optim ensuite — *sur la bonne fondation*.

---

### Pistes algorithmiques

- **Tie-breaking** : départager les `f` égaux par le plus petit `h` → moins de
  cellules explorées.
- **JPS** (*Jump Point Search*) sur grille uniforme : saute les cellules
  symétriques.
- **Hiérarchique** (clusters / portails) pour les grandes cartes.
- **Cache de chemins** + **requêtes par lots** (plusieurs NPC, même zone).

---

### Garde-fous *(quand on refactore)*

- Ne pas perdre le **profiling** (`PROFILE_ZONE` / Tracy).
- Garder la **const-correctness**.
- Ne pas oublier les **resets d'état** (ex. l'index de waypoint d'un `Path`).

> Refactor pour l'**architecture**, mais **mesure** avant/après.

---
<!-- .slide: data-background="_images/01_slide_fond_GP_22_08_22.jpg" -->
## Bilan

<div style="color:#fff;">

- A\* = Dijkstra **guidé** : `f = g + h`, heuristique **admissible**.
- L'implé `FindPath` : `mdspan` (vue zéro copie) + `std::expected` (erreur
  typée) + template (zéro coût) + tableaux plats (façon arène).
- Prochaines marches : **réutiliser** les buffers, **mutualiser** la recherche,
  puis algorithmes avancés (JPS, hiérarchique).

</div>

<small style="color:#fff;">À vous (branche 925) : brancher un `FindPath` sur votre grille de tuiles, piloté par un nœud d'action du Behaviour Tree.</small>
