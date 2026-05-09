#ifndef API_GRAPHICS_CAMERA_H_
#define API_GRAPHICS_CAMERA_H_

#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

namespace sf {
class RenderWindow;
}

namespace api::graphics {

class Camera {
 public:
  Camera() = default;

  void Setup(sf::Vector2u window_size);
  void HandleEvent(const sf::Event& event, const sf::RenderWindow& window);
  void Update(float dt);

  void Apply(sf::RenderWindow& window) const;
  void ApplyUi(sf::RenderWindow& window) const;

  const sf::View& WorldView() const { return world_view_; }
  const sf::View& UiView() const { return ui_view_; }
  float Zoom() const { return zoom_; }

 private:
  static constexpr float kPanSpeed = 600.f;
  static constexpr float kZoomMin = 0.25f;
  static constexpr float kZoomMax = 4.0f;
  static constexpr float kZoomStep = 1.1f;

  sf::View world_view_;
  sf::View ui_view_;
  sf::Vector2f drag_anchor_world_{};
  float zoom_ = 1.0f;
  bool dragging_ = false;
};

}  // namespace api::graphics

#endif  // API_GRAPHICS_CAMERA_H_
