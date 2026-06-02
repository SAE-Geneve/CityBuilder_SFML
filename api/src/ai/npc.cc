#include "ai/npc.h"

#include <algorithm>
#include <string>

#include "ai/bt_builder.h"

namespace api::ai {

using core::ai::behaviour_tree::Status;
using core::ai::behaviour_tree::TreeBuilder;

void Npc::Setup(std::string_view sprite_path, const sf::Vector2f world_size,
                const sf::Vector2f start_position) {
  world_size_ = world_size;

  if (texture_->loadFromFile(std::string(sprite_path))) {
    sprite_ = sf::Sprite(*texture_);
  }

  motor_.SetPosition(start_position);
  motor_.SetDestination(start_position);  // stay put until the first pick
  motor_.SetSpeed(kSpeed);

  // Priority behaviour, built with the fluent TreeBuilder:
  //   Selector(
  //     Sequence( IsTired, Rest ),                       // rest when tired
  //     Sequence( PickRandomDestination, MoveToDestination ) )  // else wander
  // The Selector tries to rest first; IsTired fails while energy is high, so
  // the NPC falls through to wandering. Moving drains energy, resting refills.
  bt_root_ = TreeBuilder()
                 .Selector()
                     .Sequence()
                         .Action([this] { return IsTired(); })
                         .Action([this] { return Rest(); })
                     .End()
                     .Sequence()
                         .Action([this] { return PickRandomDestination(); })
                         .Action([this] { return MoveToDestination(); })
                     .End()
                 .End()
                 .Build();
}

void Npc::Update(const float dt) {
  tick_dt_ = dt;
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

Status Npc::MoveToDestination() {
  if (motor_.RemainingDistance() <= 0.001f) {
    return Status::kSuccess;
  }
  energy_ = std::max(0.f, energy_ - kEnergyDrain * tick_dt_);
  return Status::kRunning;
}

Status Npc::IsTired() const {
  return energy_ <= kTiredThreshold ? Status::kSuccess : Status::kFailure;
}

Status Npc::Rest() {
  energy_ = std::min(kMaxEnergy, energy_ + kEnergyRegen * tick_dt_);
  return energy_ >= kMaxEnergy ? Status::kSuccess : Status::kRunning;
}

}  // namespace api::ai
