
#include <gtest/gtest.h>
#include "vec2f.h"

TEST(Vec2f, Empty) {
  core::Vec2f v;
  EXPECT_FLOAT_EQ(0, v.x);
  EXPECT_FLOAT_EQ(0, v.y);
}
