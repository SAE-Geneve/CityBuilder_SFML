//
// Created by sebas on 19/06/2025.
//

#include "ai/npc_behaviour_tree.h"

#include <functional>
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

using namespace core::ai::behaviour_tree;
using namespace api::motion;

namespace api::ai {

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
  // std::cout << "this ? = " << this << "\n";
  // std::cout << "Am I hungry ? " << std::to_string(hunger_);

  if (hunger_ >= 100) {
    // std::cout << " : Yes, I need to find food\n";

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

  } else {
    // std::cout << " : No, I can wait\n";
    return Status::kFailure;
  }
}

Status NpcBehaviourTree::Move() const {
  PROFILE_ZONE();
  // if destination not reachable, return failure
  if (!path_->valid()) {
    // std::cout << "Not reachable" << path_->IsValid() << "\n";
    return Status::kFailure;
  } else {
    // std::cout << "I'm moving" << "\n";
    if (!path_->IsDone()) {
      // still arriving, return running
      return Status::kRunning;
    } else {
      // if destination reached, return success
      return Status::kSuccess;
    }
  }
}

Status NpcBehaviourTree::Eat() {
  PROFILE_ZONE();
  // No failure, until we have food storage system
  hunger_ -= kHungerRate * tick_dt;
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
    current_ressource_ = picked_resource;
    set_destination(tilemap_->screen_position(static_cast<size_t>(current_ressource_->tile_index())));

    if (path_->valid()) return Status::kSuccess;
  }

  return Status::kFailure;
}

Status NpcBehaviourTree::GetResource() {
  PROFILE_ZONE();
  if (current_ressource_ == nullptr) return Status::kFailure;
  if (current_ressource_->quantity() <= 0) {
    current_ressource_ = nullptr;
    return Status::kSuccess;
  }

  current_ressource_->Exploit(kExploitRate * tick_dt);
  hunger_ += kHungerRate * tick_dt;
  return Status::kRunning;
}

Status NpcBehaviourTree::Idle() {
  PROFILE_ZONE();
  hunger_ += kHungerRate * tick_dt;
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


  auto feedSequence = std::make_unique<Sequence>();
  auto foodPlaceSelection = std::make_unique<Selector>();
  feedSequence->AddChild(
      std::make_unique<Action>([this]() { return CheckHunger(); }));
  feedSequence->AddChild(std::move(foodPlaceSelection));
  feedSequence->AddChild(std::make_unique<Action>([this]() { return Move(); }));
  feedSequence->AddChild(std::make_unique<Action>([this]() { return Eat(); }));

  auto workSequence = std::make_unique<Sequence>();
  workSequence->AddChild(
      std::make_unique<Action>([this]() { return PickResource(); }));
  workSequence->AddChild(std::make_unique<Action>([this]() { return Move(); }));
  workSequence->AddChild(
      std::make_unique<Action>([this]() { return GetResource(); }));

  auto selector = std::make_unique<Selector>();
  // Attach the sequence to the selector
  selector->AddChild(std::move(feedSequence));
  selector->AddChild(std::move(workSequence));
  selector->AddChild(std::make_unique<Action>([this]() { return Idle(); }));

  bt_root_ = std::move(selector);
}

void NpcBehaviourTree::Update(float dt) {
  PROFILE_ZONE();
  tick_dt = dt;
  bt_root_->Tick();
  // std::cout << "this ? = " << this << "\n";
}
}  // namespace api::ai
