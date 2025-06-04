#include "gtest/gtest.h"
#include "maths/angle.h"
#include "maths/func.h"

TEST(Vec2f, Conversion) {
  core::Degree d2 {45.0f};
  core::Degree degree{45.0f};
  auto result = core::Cos(static_cast<core::Radian>(degree));
  core::Sin(core::Degree(45.0f));
  EXPECT_FLOAT_EQ(result, std::cos(core::Pi/4.0f));
}