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
  auto path = core::ai::pathfinding::FindPath(
      std::mdspan(tilemap.data(), 3, 3), core::maths::Vec2i(),
      core::maths::Vec2i(2, 2));
  ASSERT_TRUE(path.has_value());
  EXPECT_FALSE(path->empty());
}

TEST(Pathfinding, OutOfBoundsReportsErrorCode) {
  std::array<Tile, 9> tilemap{};
  std::ranges::fill(tilemap, Tile(true));
  {
    auto path = core::ai::pathfinding::FindPath(
        std::mdspan(tilemap.data(), 3, 3), core::maths::Vec2i(-1, 0),
        core::maths::Vec2i(2, 2));
    ASSERT_FALSE(path.has_value());
    EXPECT_EQ(path.error(),
              core::ai::pathfinding::PathError::kStartOutOfBounds);
  }
  {
    auto path = core::ai::pathfinding::FindPath(
        std::mdspan(tilemap.data(), 3, 3), core::maths::Vec2i(3, 0),
        core::maths::Vec2i(2, 2));
    ASSERT_FALSE(path.has_value());
    EXPECT_EQ(path.error(),
              core::ai::pathfinding::PathError::kStartOutOfBounds);
  }
  {
    auto path = core::ai::pathfinding::FindPath(
        std::mdspan(tilemap.data(), 3, 3), core::maths::Vec2i(0, 0),
        core::maths::Vec2i(2, -2));
    ASSERT_FALSE(path.has_value());
    EXPECT_EQ(path.error(), core::ai::pathfinding::PathError::kEndOutOfBounds);
  }
  {
    auto path = core::ai::pathfinding::FindPath(
        std::mdspan(tilemap.data(), 3, 3), core::maths::Vec2i(0, 0),
        core::maths::Vec2i(2, 3));
    ASSERT_FALSE(path.has_value());
    EXPECT_EQ(path.error(), core::ai::pathfinding::PathError::kEndOutOfBounds);
  }
}

TEST(Pathfinding, StartNotWalkable) {
  std::array<Tile, 9> tilemap{};
  std::ranges::fill(tilemap, Tile(true));
  tilemap[0] = Tile(false);  // (0,0) blocked
  auto path = core::ai::pathfinding::FindPath(
      std::mdspan(tilemap.data(), 3, 3), core::maths::Vec2i(0, 0),
      core::maths::Vec2i(2, 2));
  ASSERT_FALSE(path.has_value());
  EXPECT_EQ(path.error(), core::ai::pathfinding::PathError::kStartNotWalkable);
}

TEST(Pathfinding, EndNotWalkable) {
  std::array<Tile, 9> tilemap{};
  std::ranges::fill(tilemap, Tile(true));
  tilemap[8] = Tile(false);  // (2,2) blocked
  auto path = core::ai::pathfinding::FindPath(
      std::mdspan(tilemap.data(), 3, 3), core::maths::Vec2i(0, 0),
      core::maths::Vec2i(2, 2));
  ASSERT_FALSE(path.has_value());
  EXPECT_EQ(path.error(), core::ai::pathfinding::PathError::kEndNotWalkable);
}

TEST(Pathfinding, NoPathFound) {
  // 3x3 grid where the entire middle column (x=1) is non-walkable, fully
  // separating the start (left column) from the end (right column).
  std::array<Tile, 9> tilemap{};
  std::ranges::fill(tilemap, Tile(true));
  tilemap[1 * 3 + 0] = Tile(false);  // (1,0)
  tilemap[1 * 3 + 1] = Tile(false);  // (1,1)
  tilemap[1 * 3 + 2] = Tile(false);  // (1,2)
  auto path = core::ai::pathfinding::FindPath(
      std::mdspan(tilemap.data(), 3, 3), core::maths::Vec2i(0, 0),
      core::maths::Vec2i(2, 2));
  ASSERT_FALSE(path.has_value());
  EXPECT_EQ(path.error(), core::ai::pathfinding::PathError::kNoPathFound);
}

TEST(Pathfinding, NonSquareGrid) {
  // 4-wide x 3-high grid (extent(0)=4, extent(1)=3). All walkable.
  // Layout-right: tilemap[x, y] -> data[x * 3 + y].
  // Verifies the indexing fix for width != height: shortest path from
  // (0,0) to (3,2) has Manhattan distance 5, so 6 nodes including endpoints.
  std::array<Tile, 12> tilemap{};
  std::ranges::fill(tilemap, Tile(true));
  auto path = core::ai::pathfinding::FindPath(
      std::mdspan(tilemap.data(), 4, 3), core::maths::Vec2i(0, 0),
      core::maths::Vec2i(3, 2));
  ASSERT_TRUE(path.has_value());
  EXPECT_EQ(path->size(), 6);
  EXPECT_EQ(path->front(), core::maths::Vec2i(0, 0));
  EXPECT_EQ(path->back(), core::maths::Vec2i(3, 2));
}

TEST(Pathfinding, NonSquareGridWithObstacle) {
  // 4-wide x 3-high grid. Block (1, 0) and (1, 1) so the only route from
  // (0, 0) to (3, 0) goes around through y=2.
  std::array<Tile, 12> tilemap{};
  std::ranges::fill(tilemap, Tile(true));
  tilemap[1 * 3 + 0] = Tile(false);  // (1, 0)
  tilemap[1 * 3 + 1] = Tile(false);  // (1, 1)
  auto path = core::ai::pathfinding::FindPath(
      std::mdspan(tilemap.data(), 4, 3), core::maths::Vec2i(0, 0),
      core::maths::Vec2i(3, 0));
  ASSERT_TRUE(path.has_value());
  EXPECT_EQ(path->front(), core::maths::Vec2i(0, 0));
  EXPECT_EQ(path->back(), core::maths::Vec2i(3, 0));
  // None of the path tiles should be a blocked one.
  for (const auto& p : *path) {
    EXPECT_FALSE(p == core::maths::Vec2i(1, 0));
    EXPECT_FALSE(p == core::maths::Vec2i(1, 1));
  }
}
