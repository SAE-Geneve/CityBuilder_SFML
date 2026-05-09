#include "ai/npc_behaviour_tree.h"

#include <random>
#include <utility>

#include "ai/bt_action.h"
#include "ai/bt_selector.h"
#include "ai/bt_sequence.h"
#include "ai/npc.h"
#include "motion/a_star.h"
#include "motion/path.h"
#include "profiling/profiling.h"
#include "utils/log.h"

namespace api::ai {

using core::ai::behaviour_tree::Action;
using core::ai::behaviour_tree::Selector;
using core::ai::behaviour_tree::Sequence;
using core::ai::behaviour_tree::Status;
using motion::Motor;
using motion::Path;
namespace Astar = motion::Astar;

void NpcBehaviourTree::set_destination(const sf::Vector2f& destination) const {
  PROFILE_ZONE();
  auto result =
      Astar::GetPath(*tilemap_, npc_motor_->position(), destination);
  if (!result) {
    core::LogWarning("Pathfinding failed: {}",
                     static_cast<int>(result.error()));
    path_->Fill({});
    return;
  }
  path_->Fill(result->Points());
  npc_motor_->set_destination(result->StartPoint());
}

Status NpcBehaviourTree::CheckHunger() const {
  PROFILE_ZONE();
  if (hunger_ >= 100) {
    if (!tilemap_) {
      core::LogError("No tilemap");
      return Status::kFailure;
    }
    if (!path_) {
      core::LogError("No path");
      return Status::kFailure;
    }
    if (!npc_motor_) {
      core::LogError("No motor");
      return Status::kFailure;
    }

    set_destination(cantina_position_);

    return Status::kSuccess;
  }
  return Status::kFailure;
}

Status NpcBehaviourTree::Move() const {
  PROFILE_ZONE();
  if (!path_->IsValid()) {
    return Status::kFailure;
  }
  if (!path_->IsDone()) {
    return Status::kRunning;
  }
  return Status::kSuccess;
}

Status NpcBehaviourTree::Eat() {
  PROFILE_ZONE();
  // No failure, until we have food storage system
  hunger_ -= kHungerRate * tick_dt_;
  if (hunger_ > 0) {
    return Status::kRunning;
  } else {
    return Status::kSuccess;
  }
}

Status NpcBehaviourTree::PickResource() {
  PROFILE_ZONE();
  auto resources = resource_manager_.resources(GetResourceType(npc_->type()));
  if (resources.empty()) {
    core::LogWarning("No resources available");
    return Status::kFailure;
  }

  static std::mt19937 gen{std::random_device{}()};

  auto resources_ptrs =
      resources | std::views::transform([](auto& x) { return &x; });
  resource::Resource* picked_resource = nullptr;
  std::ranges::sample(resources_ptrs, &picked_resource, 1,
                      gen);  // C++20 ranges form
  if (picked_resource != nullptr && picked_resource->quantity() > 0) {
    current_resource_ = picked_resource;
    set_destination(tilemap_->screen_position(static_cast<size_t>(current_resource_->tile_index())));

    if (path_->IsValid()) return Status::kSuccess;
  }

  return Status::kFailure;
}

Status NpcBehaviourTree::GetResource() {
  PROFILE_ZONE();
  if (current_resource_ == nullptr) return Status::kFailure;
  if (current_resource_->quantity() <= 0) {
    current_resource_ = nullptr;
    return Status::kSuccess;
  }

  current_resource_->Exploit(kExploitRate * tick_dt_);
  hunger_ += kHungerRate * tick_dt_;
  return Status::kRunning;
}

Status NpcBehaviourTree::Idle() {
  PROFILE_ZONE();
  hunger_ += kHungerRate * tick_dt_;
  core::LogDebug("I'm sleeping");
  return Status::kSuccess;
}

void NpcBehaviourTree::SetupBehaviourTree(Motor* npc_motor, Path* path,
                                          Npc* npc, const TileMap* tilemap,
                                          sf::Vector2f cantina_position) {
  PROFILE_ZONE();
  core::LogDebug("Setup Behaviour Tree");

  hunger_ = 0;
  npc_ = npc;
  npc_motor_ = npc_motor;
  path_ = path;
  tilemap_ = tilemap;
  cantina_position_ = cantina_position;


  auto feed_sequence = std::make_unique<Sequence>();
  auto food_place_selection = std::make_unique<Selector>();
  feed_sequence->AddChild(
      std::make_unique<Action>([this]() { return CheckHunger(); }));
  feed_sequence->AddChild(std::move(food_place_selection));
  feed_sequence->AddChild(std::make_unique<Action>([this]() { return Move(); }));
  feed_sequence->AddChild(std::make_unique<Action>([this]() { return Eat(); }));

  auto work_sequence = std::make_unique<Sequence>();
  work_sequence->AddChild(
      std::make_unique<Action>([this]() { return PickResource(); }));
  work_sequence->AddChild(std::make_unique<Action>([this]() { return Move(); }));
  work_sequence->AddChild(
      std::make_unique<Action>([this]() { return GetResource(); }));

  auto selector = std::make_unique<Selector>();
  // Attach the sequence to the selector
  selector->AddChild(std::move(feed_sequence));
  selector->AddChild(std::move(work_sequence));
  selector->AddChild(std::make_unique<Action>([this]() { return Idle(); }));

  bt_root_ = std::move(selector);
}

void NpcBehaviourTree::Update(float dt) {
  PROFILE_ZONE();
  tick_dt_ = dt;
  bt_root_->Tick();
}
}  // namespace api::ai
