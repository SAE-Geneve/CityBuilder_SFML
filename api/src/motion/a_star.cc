#include "motion/a_star.h"

#include <cmath>

#include "ai/a_star.h"
#include "graphics/tilemap.h"
#include "motion/path.h"
#include "profiling/profiling.h"

namespace api::motion::Astar {

namespace {

core::maths::Vec2i ScreenToGrid(sf::Vector2f screen_pos, int step) {
  return {static_cast<int>(std::lround(screen_pos.x / static_cast<float>(step))),
          static_cast<int>(std::lround(screen_pos.y / static_cast<float>(step)))};
}

sf::Vector2f GridToScreen(core::maths::Vec2i grid, int step) {
  return {static_cast<float>(grid.x * step),
          static_cast<float>(grid.y * step)};
}

}  // namespace

std::expected<Path, core::ai::pathfinding::PathError> GetPath(
    const TileMap& tilemap, sf::Vector2f start, sf::Vector2f end) {
  PROFILE_ZONE();
  const int step = TileMap::step();
  const auto start_grid = ScreenToGrid(start, step);
  const auto end_grid = ScreenToGrid(end, step);

  auto grid_path =
      core::ai::pathfinding::FindPath(tilemap.AsMdspan(), start_grid, end_grid);
  if (!grid_path) {
    return std::unexpected(grid_path.error());
  }

  std::vector<sf::Vector2f> screen_points;
  screen_points.reserve(grid_path->size());
  for (const auto& cell : *grid_path) {
    screen_points.push_back(GridToScreen(cell, step));
  }

  Path path;
  path.Fill(screen_points);
  return path;
}

}  // namespace api::motion::Astar
