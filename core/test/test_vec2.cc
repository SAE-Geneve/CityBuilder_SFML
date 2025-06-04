
#include "gtest/gtest.h"
#include "maths/vec2.h"
#include "maths/vec2f.h"
#include <SFML/System/Vector2.hpp>

TEST(Vec2, Empty) {
  core::Vec2f v1{1.2f, 1.3f};
  sf::Vector2f v2 = sf::Vector2f(1.2f, 1.3f);
  core::Vec2<float> v { v2 };

  sf::Vector2f v3 = static_cast<sf::Vector2f>(v);
}
