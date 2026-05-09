#ifndef API_MOTION_MOTOR_H_
#define API_MOTION_MOTOR_H_

#include <SFML/System/Vector2.hpp>

namespace api::motion {
class Motor {
  sf::Vector2f position_;
  sf::Vector2f destination_;
  float speed_ = 0;

  float remainingDistance_ = 0;

 public:
  void Update(float dt);  // each frame
  [[nodiscard]] float remaining_distance() const;

  // getter / setter
  void set_speed(float speed) { speed_ = speed; }
  void set_position(sf::Vector2f position) { position_ = position; }

  void set_destination(const sf::Vector2f destination) {
    destination_ = destination;
  }

  [[nodiscard]] const sf::Vector2f& position() const { return position_; }
};

inline void Motor::Update(const float dt) {
  const sf::Vector2f distance = destination_ - position_;
  remainingDistance_ = distance.length();

  if (remainingDistance_ < speed_ * dt) {
    position_ = destination_;
    return;
  }
  position_ += distance.normalized() * speed_ * dt;
}

inline float Motor::remaining_distance() const { return remainingDistance_; }
}  // namespace api::motion

#endif  // API_MOTION_MOTOR_H_
