#include "gtest/gtest.h"
#include "maths/angle.h"
#include "maths/func.h"

TEST(Vec2f, Conversion) {
  core::maths::Degree d2{45.0f};
  core::maths::Degree degree{45.0f};
  auto result = core::maths::Cos(static_cast<core::maths::Radian>(degree));
  core::maths::Sin(core::maths::Degree(45.0f));
  EXPECT_FLOAT_EQ(result, std::cos(core::maths::Pi / 4.0f));
}