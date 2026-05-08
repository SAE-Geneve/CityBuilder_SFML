#include "graphics/camera.h"

#include <algorithm>
#include <cmath>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "profiling/profiling.h"

namespace api::graphics {

void Camera::Setup(sf::Vector2u window_size) {
  PROFILE_ZONE();
  const sf::Vector2f size{static_cast<float>(window_size.x),
                          static_cast<float>(window_size.y)};
  const sf::Vector2f center = size / 2.f;
  world_view_ = sf::View(center, size);
  ui_view_ = sf::View(center, size);
  zoom_ = 1.0f;
  dragging_ = false;
}

void Camera::HandleEvent(const sf::Event& event,
                         const sf::RenderWindow& window) {
  PROFILE_ZONE();
  if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>();
      pressed && pressed->button == sf::Mouse::Button::Middle) {
    dragging_ = true;
    drag_anchor_world_ =
        window.mapPixelToCoords(pressed->position, world_view_);
    return;
  }

  if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>();
      released && released->button == sf::Mouse::Button::Middle) {
    dragging_ = false;
    return;
  }

  if (const auto* moved = event.getIf<sf::Event::MouseMoved>();
      moved && dragging_) {
    const sf::Vector2f current =
        window.mapPixelToCoords(moved->position, world_view_);
    world_view_.move(drag_anchor_world_ - current);
    return;
  }

  if (const auto* scroll = event.getIf<sf::Event::MouseWheelScrolled>();
      scroll) {
    const float factor = (scroll->delta > 0) ? (1.f / kZoomStep) : kZoomStep;
    const float new_zoom = std::clamp(zoom_ * factor, kZoomMin, kZoomMax);
    if (new_zoom == zoom_) return;
    const float effective = new_zoom / zoom_;
    const sf::Vector2f before =
        window.mapPixelToCoords(scroll->position, world_view_);
    world_view_.zoom(effective);
    zoom_ = new_zoom;
    const sf::Vector2f after =
        window.mapPixelToCoords(scroll->position, world_view_);
    world_view_.move(before - after);
    return;
  }
}

void Camera::Update(float dt) {
  PROFILE_ZONE();
  sf::Vector2f dir{};
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
    dir.y -= 1.f;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
    dir.y += 1.f;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
    dir.x -= 1.f;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
    dir.x += 1.f;
  }

  if (dir.x == 0.f && dir.y == 0.f) return;

  const float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
  dir /= length;
  world_view_.move(dir * kPanSpeed * zoom_ * dt);
}

void Camera::Apply(sf::RenderWindow& window) const {
  window.setView(world_view_);
}

void Camera::ApplyUi(sf::RenderWindow& window) const {
  window.setView(ui_view_);
}

}  // namespace api::graphics
