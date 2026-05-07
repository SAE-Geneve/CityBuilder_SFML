//
// Created by unite on 26.02.2026.
//

#include "ai/a_star.h"
#include "gtest/gtest.h"

struct Tile {
  bool is_walkable = false;

  [[nodiscard]] constexpr bool IsWalkable() const noexcept {
    return is_walkable;
  }
};

TEST(Pathfinding, Empty) {
  std::array<Tile, 9> tilemap{};
  std::ranges::fill(tilemap, Tile(true));
  auto path = core::ai::experimental::calculate_shortest_path(
      std::mdspan(tilemap.data(), 3, 3), core::maths::Vec2i(),
      core::maths::Vec2i(2, 2));
  ASSERT_TRUE(path.has_value());
  EXPECT_FALSE(path->empty());
}

TEST(Pathfinding, OutOfBoundsReportsErrorCode) {
  std::array<Tile, 9> tilemap{};
  std::ranges::fill(tilemap, Tile(true));
  {
    auto path = core::ai::experimental::calculate_shortest_path(
        std::mdspan(tilemap.data(), 3, 3), core::maths::Vec2i(-1, 0),
        core::maths::Vec2i(2, 2));
    ASSERT_FALSE(path.has_value());
    EXPECT_EQ(path.error(),
              core::ai::experimental::PathError::kStartOutOfBounds);
  }
  {
    auto path = core::ai::experimental::calculate_shortest_path(
        std::mdspan(tilemap.data(), 3, 3), core::maths::Vec2i(3, 0),
        core::maths::Vec2i(2, 2));
    ASSERT_FALSE(path.has_value());
    EXPECT_EQ(path.error(),
              core::ai::experimental::PathError::kStartOutOfBounds);
  }
  {
    auto path = core::ai::experimental::calculate_shortest_path(
        std::mdspan(tilemap.data(), 3, 3), core::maths::Vec2i(0, 0),
        core::maths::Vec2i(2, -2));
    ASSERT_FALSE(path.has_value());
    EXPECT_EQ(path.error(), core::ai::experimental::PathError::kEndOutOfBounds);
  }
  {
    auto path = core::ai::experimental::calculate_shortest_path(
        std::mdspan(tilemap.data(), 3, 3), core::maths::Vec2i(0, 0),
        core::maths::Vec2i(2, 3));
    ASSERT_FALSE(path.has_value());
    EXPECT_EQ(path.error(), core::ai::experimental::PathError::kEndOutOfBounds);
  }
}

TEST(Pathfinding, StartNotWalkable) {
  std::array<Tile, 9> tilemap{};
  std::ranges::fill(tilemap, Tile(true));
  tilemap[0] = Tile(false);  // (0,0) blocked
  auto path = core::ai::experimental::calculate_shortest_path(
      std::mdspan(tilemap.data(), 3, 3), core::maths::Vec2i(0, 0),
      core::maths::Vec2i(2, 2));
  ASSERT_FALSE(path.has_value());
  EXPECT_EQ(path.error(), core::ai::experimental::PathError::kStartNotWalkable);
}

TEST(Pathfinding, EndNotWalkable) {
  std::array<Tile, 9> tilemap{};
  std::ranges::fill(tilemap, Tile(true));
  tilemap[8] = Tile(false);  // (2,2) blocked
  auto path = core::ai::experimental::calculate_shortest_path(
      std::mdspan(tilemap.data(), 3, 3), core::maths::Vec2i(0, 0),
      core::maths::Vec2i(2, 2));
  ASSERT_FALSE(path.has_value());
  EXPECT_EQ(path.error(), core::ai::experimental::PathError::kEndNotWalkable);
}

TEST(Pathfinding, NoPathFound) {
  // 3x3 grid where the entire middle column (x=1) is non-walkable, fully
  // separating the start (left column) from the end (right column).
  std::array<Tile, 9> tilemap{};
  std::ranges::fill(tilemap, Tile(true));
  tilemap[1 * 3 + 0] = Tile(false);  // (1,0)
  tilemap[1 * 3 + 1] = Tile(false);  // (1,1)
  tilemap[1 * 3 + 2] = Tile(false);  // (1,2)
  auto path = core::ai::experimental::calculate_shortest_path(
      std::mdspan(tilemap.data(), 3, 3), core::maths::Vec2i(0, 0),
      core::maths::Vec2i(2, 2));
  ASSERT_FALSE(path.has_value());
  EXPECT_EQ(path.error(), core::ai::experimental::PathError::kNoPathFound);
}
