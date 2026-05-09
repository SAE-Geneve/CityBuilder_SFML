//
// Created by sebas on 25/06/2025.
//

#include "ui/button.h"

#include <SFML/Graphics/Texture.hpp>

#include "maths/vec2.h"
#include "profiling/profiling.h"
#include "utils/log.h"

namespace api::ui {

Button::Button(const sf::Vector2f pos, std::string_view label,
               const sf::Texture& t, const sf::Texture& hover_t,
               const sf::Font& font)
    : Clickable(), sprite_(t), hover_sprite_(hover_t), label_(font) {
  PROFILE_ZONE();
  sf::Vector2i int_size = {static_cast<int>(sprite_.getTexture().getSize().x),
                           static_cast<int>(sprite_.getTexture().getSize().y)};
  sf::Vector2i int_pos = {static_cast<int>(pos.x), static_cast<int>(pos.y)};

  core::LogDebug("Texture size : {}:{}", int_size.x, int_size.y);

  sprite_.setPosition(pos);
  hover_sprite_.setPosition(pos);

  label_.setCharacterSize(24);
  label_.setString(std::string(label));
  label_.setOrigin(label_.getLocalBounds().getCenter());
  label_.setPosition(static_cast<sf::Vector2f>(
      core::maths::Vec2f{pos} +
      core::maths::Vec2f{sprite_.getTexture().getSize()} / 2.0f));

  Button::SetZone(sf::IntRect(int_pos, int_size));

  OnHoverEnter = [this]() {
    sprite_.setColor(sf::Color(0, 0, 0, 0));
    hover_sprite_.setColor(sf::Color(255, 255, 255, 255));
    label_.setFillColor(sf::Color::Black);
  };
  OnHoverExit = [this]() {
    sprite_.setColor(sf::Color(255, 255, 255, 255));
    hover_sprite_.setColor(sf::Color(0, 0, 0, 0));
    label_.setFillColor(sf::Color::White);
  };

  sprite_.setColor(sf::Color(255, 255, 255, 255));
  hover_sprite_.setColor(sf::Color(0, 0, 0, 0));
}

void Button::Draw(sf::RenderWindow& window) const {
  PROFILE_ZONE();
  window.draw(sprite_);
  window.draw(hover_sprite_);
  window.draw(label_);
}
}  // namespace api::ui
