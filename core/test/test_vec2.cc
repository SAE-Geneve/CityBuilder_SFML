
#include <SFML/System/Vector2.hpp>

#include "gtest/gtest.h"
#include "maths/vec2.h"

TEST(Vec2, Empty) {
  core::maths::Vec2f v1{1.2f, 1.3f};
  sf::Vector2f v2 = sf::Vector2f(1.2f, 1.3f);
  core::maths::Vec2<float> v{v2};

  sf::Vector2f v3 = static_cast<sf::Vector2f>(v);
}
