//
// Created by unite on 26.02.2026.
//

#ifndef CITYBUILDER_A_STAR_H
#define CITYBUILDER_A_STAR_H

#include <algorithm>
#include <climits>
#include <expected>
#include <mdspan>
#include <queue>
#include <vector>

#include "maths/vec2.h"
#include "profiling/profiling.h"

namespace core::ai::pathfinding {

enum class PathError {
  kStartOutOfBounds,
  kEndOutOfBounds,
  kStartNotWalkable,
  kEndNotWalkable,
  kNoPathFound,
};

template <typename tile_t, typename extents_t>
std::expected<std::vector<maths::Vec2i>, PathError> FindPath(
    std::mdspan<tile_t, extents_t> tilemap, maths::Vec2i start_pos,
    maths::Vec2i end_pos) {
  PROFILE_ZONE();
  // extent(0) is the x-dimension (first index), extent(1) is the y-dimension
  // (second index, fastest-varying for layout-right). The y-extent is the row
  // stride for flat-index conversion.
  const int width = static_cast<int>(tilemap.extent(0));
  const int height = static_cast<int>(tilemap.extent(1));

  if (start_pos.x < 0 || start_pos.x >= width || start_pos.y < 0 ||
      start_pos.y >= height) {
    return std::unexpected(PathError::kStartOutOfBounds);
  }
  if (end_pos.x < 0 || end_pos.x >= width || end_pos.y < 0 ||
      end_pos.y >= height) {
    return std::unexpected(PathError::kEndOutOfBounds);
  }
  if (!tilemap[start_pos.x, start_pos.y].IsWalkable()) {
    return std::unexpected(PathError::kStartNotWalkable);
  }
  if (!tilemap[end_pos.x, end_pos.y].IsWalkable()) {
    return std::unexpected(PathError::kEndNotWalkable);
  }

  static constexpr std::array neighbor_dirs{
      maths::Vec2i(-1, 0), maths::Vec2i(0, 1), maths::Vec2i(1, 0),
      maths::Vec2i(0, -1)};

  const int total = width * height;
  auto to_index = [height](maths::Vec2i p) { return p.x * height + p.y; };
  auto manhattan = [](maths::Vec2i a, maths::Vec2i b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
  };

  struct Node {
    int f = 0;
    maths::Vec2i pos;
    bool operator<(const Node& other) const { return f > other.f; }
  };

  std::vector<int8_t> visited(total, 0);
  std::vector<int> came_from(total, -1);
  std::vector<int> g_cost(total, INT_MAX);
  std::priority_queue<Node> open_list;

  const int start_idx = to_index(start_pos);
  g_cost[start_idx] = 0;
  open_list.push({manhattan(start_pos, end_pos), start_pos});

  while (!open_list.empty()) {
    const auto current = open_list.top();
    open_list.pop();

    if (current.pos == end_pos) {
      std::vector<maths::Vec2i> path;
      maths::Vec2i trace = end_pos;
      while (trace != start_pos) {
        path.push_back(trace);
        const int idx = came_from[to_index(trace)];
        trace = {idx / height, idx % height};
      }
      path.push_back(start_pos);
      std::ranges::reverse(path);
      return path;
    }

    const int cur_idx = to_index(current.pos);
    if (visited[cur_idx]) {
      continue;
    }
    visited[cur_idx] = 1;

    for (const auto& dir : neighbor_dirs) {
      const auto neighbor_pos = current.pos + dir;
      if (neighbor_pos.x < 0 || neighbor_pos.x >= width ||
          neighbor_pos.y < 0 || neighbor_pos.y >= height) {
        continue;
      }
      const int n_idx = to_index(neighbor_pos);
      if (visited[n_idx] ||
          !tilemap[neighbor_pos.x, neighbor_pos.y].IsWalkable()) {
        continue;
      }
      const int tentative_g = g_cost[cur_idx] + 1;
      if (tentative_g < g_cost[n_idx]) {
        g_cost[n_idx] = tentative_g;
        came_from[n_idx] = cur_idx;
        open_list.push(
            {tentative_g + manhattan(neighbor_pos, end_pos), neighbor_pos});
      }
    }
  }

  return std::unexpected(PathError::kNoPathFound);
}

}  // namespace core::ai::pathfinding

#endif  // CITYBUILDER_A_STAR_H
