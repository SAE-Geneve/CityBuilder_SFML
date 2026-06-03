#include "ai/npc.h"

#include <format>


namespace api::ai {


using core::ai::behaviour_tree::Status;

void Npc::Setup(std::string_view sprite_path, const sf::Vector2f world_size,
                const sf::Vector2f start_position) {
  world_size_ = world_size;

  if (texture_->loadFromFile(std::string(sprite_path))) {
    sprite_ = sf::Sprite(*texture_);
  }

  motor_.SetPosition(start_position);
  motor_.SetDestination(start_position);  // stay put until the first pick
  motor_.SetSpeed(kSpeed);

  // Rough wander behaviour:
  //   Sequence( pick a random destination, then move to it )
  // PickRandomDestination always succeeds, MoveToDestination stays kRunning
  // until the motor reaches the target. When the sequence completes it resets

}

void Npc::Update(const float dt) {
  motor_.Update(dt);
  if (bt_root_) {
    bt_root_->Tick();
  }
}

void Npc::Draw(sf::RenderWindow& window) {
  if (sprite_.has_value()) {
    sprite_->setPosition(motor_.GetPosition());
    window.draw(*sprite_);
  }
}

Status Npc::PickRandomDestination() {
  std::uniform_real_distribution<float> x_dist(0.f, world_size_.x);
  std::uniform_real_distribution<float> y_dist(0.f, world_size_.y);
  motor_.SetDestination({x_dist(rng_), y_dist(rng_)});
  return Status::kSuccess;
}

Status Npc::MoveToDestination() const {
  return motor_.RemainingDistance() <= 0.001f ? Status::kSuccess
                                              : Status::kRunning;
}

}  // namespace api::ai
