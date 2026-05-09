#ifndef API_UI_CLICKABLE_H_
#define API_UI_CLICKABLE_H_

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>

namespace api::ui {
class Clickable {
 private:
  sf::IntRect zone_;
  bool is_hover_ = false;

 protected:
  // Constructeur protected, la classe demande un heritage
  Clickable() = default;

  // TODO(forbidden-features): std::function is banned in this codebase.
  // Replace with virtual hooks or function pointers + context.
  std::function<void()> on_hover_enter;
  std::function<void()> on_hover_exit;

 public:
  virtual ~Clickable() = default;
  // TODO(google-style): replace `bool& wasClicked` with `bool*` per
  // Google output-parameter convention.
  virtual void HandleEvent(std::optional<sf::Event> event, bool& wasClicked);

  virtual void SetZone(sf::IntRect zone);

  std::function<void()> on_released_left;
  std::function<void()> on_released_right;
  std::function<void()> on_pressed_left;
  std::function<void()> on_pressed_right;
};
}  // namespace api::ui

#endif  // API_UI_CLICKABLE_H_