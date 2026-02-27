//
// Created by unite on 26.02.2026.
//

#include "ai/a_star.h"
#include "gtest/gtest.h"

struct Tile {
  bool is_walkable = false;

  [[nodiscard]] constexpr bool IsWalkable() const noexcept { return is_walkable; }
};

TEST(Pathfinding, Empty) {

  std::array<Tile, 9> tilemap{};
  std::ranges::fill(tilemap, Tile(true));
  auto path = core::ai::experimental::calculate_shortest_path(std::mdspan(tilemap.data(), 3, 3), core::Vec2i(), core::Vec2i(2,2));
  EXPECT_FALSE(path.empty());

}