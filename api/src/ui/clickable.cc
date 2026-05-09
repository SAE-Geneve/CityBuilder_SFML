//
// Created by sebas on 25/06/2025.
//

#include "ui/clickable.h"

#include "profiling/profiling.h"

namespace api::ui {
void Clickable::HandleEvent(std::optional<sf::Event> evt, bool& wasClicked) {
  PROFILE_ZONE();
  const auto released = evt->getIf<sf::Event::MouseButtonReleased>();
  if (released && !wasClicked) {
    if (zone_.contains(released->position)) {
      if (released->button == sf::Mouse::Button::Left) {
        if (on_released_left) {
          on_released_left();
          wasClicked |= true;  // Indicate that a click was handled
        }
      }

      if (released->button == sf::Mouse::Button::Right) {
        if (on_released_right) {
          on_released_right();
          wasClicked |= true;  // Indicate that a click was handled
        }
      }
    }
  }

  const auto pressed = evt->getIf<sf::Event::MouseButtonPressed>();
  if (pressed && !wasClicked) {
    if (zone_.contains(pressed->position)) {
      if (pressed->button == sf::Mouse::Button::Left) {
        if (on_pressed_left) {
          on_pressed_left();
          wasClicked |= true;  // Indicate that a click was handled
        }
      }

      if (pressed->button == sf::Mouse::Button::Right) {
        if (on_pressed_right) {
          on_pressed_right();
          wasClicked |= true;  // Indicate that a click was handled
        }
      }
    }
  }

  const auto mouse_move = evt->getIf<sf::Event::MouseMoved>();
  if (mouse_move) {
    if (zone_.contains(mouse_move->position) && !is_hover_) {
      if (on_hover_enter) on_hover_enter();
      is_hover_ = true;
    }
    if (!zone_.contains(mouse_move->position) && is_hover_) {
      if (on_hover_exit) on_hover_exit();
      is_hover_ = false;
    }
  }
}

void Clickable::SetZone(sf::IntRect zone) {
  PROFILE_ZONE();
  zone_ = zone;
}

}  // namespace api::ui
