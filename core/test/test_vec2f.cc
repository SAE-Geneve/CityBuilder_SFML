#include "gtest/gtest.h"
#include "maths/vec2f.h"

TEST(Vec2f, Empty) {
  core::Vec2f v;
  EXPECT_FLOAT_EQ(0, v.x);
  EXPECT_FLOAT_EQ(0, v.y);
}

TEST(Vec2f, Add) {
  constexpr float x1 = 1, x2 = 2;
  constexpr float y1 = 3, y2 = -4;
  constexpr core::Vec2f v1{x1, y1};
  constexpr core::Vec2f v2{x2, y2};

  constexpr auto result = v1+v2;

  static_assert(result.x == x1 + x2);
  static_assert(result.y == y1 + y2);

  EXPECT_FLOAT_EQ(result.x, x1+x2);
  EXPECT_FLOAT_EQ(result.y, y1+y2);


}