#include <SFML/System/Vector2.hpp>

#include "gtest/gtest.h"
#include "maths/vec2.h"

namespace {
using core::maths::Vec2f;
using core::maths::Vec2i;
}  // namespace

// ---------- Vec2f ----------

TEST(Vec2f, DefaultConstructIsZero) {
  Vec2f v;
  EXPECT_FLOAT_EQ(v.x, 0.0f);
  EXPECT_FLOAT_EQ(v.y, 0.0f);
}

TEST(Vec2f, ComponentConstruct) {
  Vec2f v{1.5f, -2.5f};
  EXPECT_FLOAT_EQ(v.x, 1.5f);
  EXPECT_FLOAT_EQ(v.y, -2.5f);
}

TEST(Vec2f, EqualityHoldsForSameComponents) {
  EXPECT_TRUE(Vec2f(1.0f, 2.0f) == Vec2f(1.0f, 2.0f));
}

TEST(Vec2f, EqualityFailsForDifferentComponents) {
  EXPECT_FALSE(Vec2f(1.0f, 2.0f) == Vec2f(1.0f, 2.5f));
  EXPECT_FALSE(Vec2f(1.0f, 2.0f) == Vec2f(0.0f, 2.0f));
}

TEST(Vec2f, AdditionAddsComponents) {
  const auto sum = Vec2f(1.0f, 2.0f) + Vec2f(3.0f, -1.0f);
  EXPECT_FLOAT_EQ(sum.x, 4.0f);
  EXPECT_FLOAT_EQ(sum.y, 1.0f);
}

TEST(Vec2f, SqrLengthOfZeroIsZero) {
  EXPECT_FLOAT_EQ(Vec2f{}.SqrLength(), 0.0f);
}

TEST(Vec2f, SqrLengthOf3_4Is25) {
  EXPECT_FLOAT_EQ(Vec2f(3.0f, 4.0f).SqrLength(), 25.0f);
}

TEST(Vec2f, SqrLengthIgnoresSign) {
  EXPECT_FLOAT_EQ(Vec2f(-3.0f, -4.0f).SqrLength(), 25.0f);
}

TEST(Vec2f, LengthOf3_4Is5) {
  EXPECT_FLOAT_EQ(Vec2f(3.0f, 4.0f).Length(), 5.0f);
}

TEST(Vec2f, NormalizeYieldsUnitLength) {
  Vec2f v{3.0f, 4.0f};
  v.Normalize();
  EXPECT_FLOAT_EQ(v.Length(), 1.0f);
}

TEST(Vec2f, NormalizeProducesExpectedComponents) {
  Vec2f v{3.0f, 4.0f};
  v.Normalize();
  EXPECT_FLOAT_EQ(v.x, 0.6f);
  EXPECT_FLOAT_EQ(v.y, 0.8f);
}

TEST(Vec2f, DotComputesScalarProduct) {
  EXPECT_FLOAT_EQ(Vec2f::Dot(Vec2f(1.0f, 2.0f), Vec2f(3.0f, 4.0f)), 11.0f);
}

TEST(Vec2f, DotOfOrthogonalIsZero) {
  EXPECT_FLOAT_EQ(Vec2f::Dot(Vec2f(1.0f, 0.0f), Vec2f(0.0f, 1.0f)), 0.0f);
}

// ---------- Vec2i ----------

TEST(Vec2i, DefaultConstructIsZero) {
  Vec2i v;
  EXPECT_EQ(v.x, 0);
  EXPECT_EQ(v.y, 0);
}

TEST(Vec2i, ComponentConstruct) {
  Vec2i v{3, -7};
  EXPECT_EQ(v.x, 3);
  EXPECT_EQ(v.y, -7);
}

TEST(Vec2i, EqualityHoldsForSameComponents) {
  EXPECT_TRUE(Vec2i(1, 2) == Vec2i(1, 2));
}

TEST(Vec2i, EqualityFailsForDifferentComponents) {
  EXPECT_FALSE(Vec2i(1, 2) == Vec2i(1, 3));
  EXPECT_FALSE(Vec2i(1, 2) == Vec2i(0, 2));
}

TEST(Vec2i, AdditionAddsComponents) {
  const auto sum = Vec2i(1, 2) + Vec2i(3, -5);
  EXPECT_EQ(sum.x, 4);
  EXPECT_EQ(sum.y, -3);
}

TEST(Vec2i, SqrLengthOfZeroIsZero) {
  EXPECT_EQ(Vec2i{}.SqrLength(), 0);
}

TEST(Vec2i, SqrLengthOf3_4Is25) {
  EXPECT_EQ(Vec2i(3, 4).SqrLength(), 25);
}

TEST(Vec2i, SqrLengthIgnoresSign) {
  EXPECT_EQ(Vec2i(-3, -4).SqrLength(), 25);
}

TEST(Vec2i, DotComputesScalarProduct) {
  EXPECT_EQ(Vec2i::Dot(Vec2i(1, 2), Vec2i(3, 4)), 11);
}

TEST(Vec2i, DotOfOrthogonalIsZero) {
  EXPECT_EQ(Vec2i::Dot(Vec2i(1, 0), Vec2i(0, 1)), 0);
}

// ---------- Conversions ----------

TEST(Vec2Conversion, Vec2iToVec2fIsLossless) {
  Vec2f v{Vec2i{3, -4}};
  EXPECT_FLOAT_EQ(v.x, 3.0f);
  EXPECT_FLOAT_EQ(v.y, -4.0f);
}

TEST(Vec2Conversion, Vec2fToVec2iTruncatesTowardZero) {
  Vec2i v{Vec2f{1.7f, -2.3f}};
  EXPECT_EQ(v.x, 1);
  EXPECT_EQ(v.y, -2);
}

TEST(Vec2Conversion, FromSfVector2fSameType) {
  Vec2f v{sf::Vector2f{1.5f, 2.5f}};
  EXPECT_FLOAT_EQ(v.x, 1.5f);
  EXPECT_FLOAT_EQ(v.y, 2.5f);
}

TEST(Vec2Conversion, FromSfVector2iSameType) {
  Vec2i v{sf::Vector2i{3, 4}};
  EXPECT_EQ(v.x, 3);
  EXPECT_EQ(v.y, 4);
}

TEST(Vec2Conversion, FromSfVector2iToVec2fCrossType) {
  Vec2f v{sf::Vector2i{3, 4}};
  EXPECT_FLOAT_EQ(v.x, 3.0f);
  EXPECT_FLOAT_EQ(v.y, 4.0f);
}

TEST(Vec2Conversion, FromSfVector2fToVec2iTruncates) {
  Vec2i v{sf::Vector2f{1.7f, -2.3f}};
  EXPECT_EQ(v.x, 1);
  EXPECT_EQ(v.y, -2);
}

TEST(Vec2Conversion, ToSfVector2fSameType) {
  Vec2f v{1.5f, 2.5f};
  const auto sv = static_cast<sf::Vector2f>(v);
  EXPECT_FLOAT_EQ(sv.x, 1.5f);
  EXPECT_FLOAT_EQ(sv.y, 2.5f);
}

TEST(Vec2Conversion, ToSfVector2iSameType) {
  Vec2i v{3, 4};
  const auto sv = static_cast<sf::Vector2i>(v);
  EXPECT_EQ(sv.x, 3);
  EXPECT_EQ(sv.y, 4);
}
