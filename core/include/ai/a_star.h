//
// Created by unite on 26.02.2026.
//

#ifndef CITYBUILDER_A_STAR_H
#define CITYBUILDER_A_STAR_H

#include <mdspan>
#include <vector>
#include "maths/vec2.h"

namespace core::ai::experimental {


template <typename tile_t, typename extends>
std::vector<Vec2i> calculate_shortest_path(std::mdspan<tile_t, extends> tilemap, Vec2i start_pos, Vec2i end_pos) {
  std::vector<Vec2i> result;
  if (start_pos.x <0 || start_pos.x >= tilemap.extent(0) || start_pos.y <0 || start_pos.y >= tilemap.extent(1)) {
    return result;
  }
  if (end_pos.x < 0 || end_pos.x >= tilemap.extent(0) || end_pos.y < 0 || end_pos.y >= tilemap.extent(1)) {
    return result;
  }
  if (!tilemap[start_pos.x, start_pos.y].IsWalkable() || !tilemap[end_pos.x, end_pos.y].IsWalkable()) {
    return result;
  }
  static constexpr std::array neighbor_dirs{Vec2i(-1,0),Vec2i(0,1),Vec2i(1,0),Vec2i(0,-1)};

  auto current_pos = start_pos;
  for (const auto neighbor_dir : neighbor_dirs) {
    const auto neighbor_pos = current_pos + neighbor_dir;

  }

  return {};
}

}

#endif  // CITYBUILDER_A_STAR_H
