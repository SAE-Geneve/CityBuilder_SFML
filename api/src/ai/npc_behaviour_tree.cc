//
// Created by sebas on 19/06/2025.
//

#include "ai/npc_behaviour_tree.h"

#include <functional>
#include <random>
#include <utility>

#include "utils/log.h"

#include "ai/bt_action.h"
#include "ai/bt_selector.h"
#include "ai/bt_sequence.h"
#include "motion/a_star.h"
#include "motion/path.h"

#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

using namespace core::ai::behaviour_tree;
using namespace api::motion;

namespace api::ai {

void NpcBehaviourTree::SetDestination(const sf::Vector2f& destination) const {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  Path path = Astar::GetPath(TileMap::GetStep(), npc_motor_->GetPosition(),
                             destination, tilemap_->GetWalkables());
  if (path.IsValid()) {
    this->path_->Fill(path.Points());
    this->npc_motor_->SetDestination(path.StartPoint());
  }
}

Status NpcBehaviourTree::CheckHunger() const {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
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

    SetDestination(cantina_position_);

    return Status::kSuccess;

  } else {
    // std::cout << " : No, I can wait\n";
    return Status::kFailure;
  }
}

Status NpcBehaviourTree::Move() const {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  // if destination not reachable, return failure
  if (!path_->IsValid()) {
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
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  // No failure, until we have food storage system
  hunger_ -= kHungerRate * tick_dt;
  if (hunger_ > 0) {
    return Status::kRunning;
  } else {
    return Status::kSuccess;
  }
}

Status NpcBehaviourTree::PickRessource() {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  if (ressources_.empty()) {
    core::LogWarning("No ressources available");
    return Status::kFailure;
  }

  //this static saves 2.5kb every call
  static std::mt19937 gen{std::random_device{}()};
  std::uniform_int_distribution<size_t> dist(0, ressources_.size() - 1);

  //Fixed: not throwing dice twice
  const auto new_index = dist(gen);
  if (ressources_[new_index].GetQty() > 0) {
    current_ressource_ = ressources_[new_index];
    SetDestination(TileMap::ScreenPosition(current_ressource_.GetTileIndex()));

    if (path_->IsValid()) return Status::kSuccess;
  }

  return Status::kFailure;
}

Status NpcBehaviourTree::GetRessource() {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  if (current_ressource_.GetQty() <= 0) {
    return Status::kSuccess;
  }

  current_ressource_.Exploit(kExploitRate * tick_dt);
  hunger_ += kHungerRate * tick_dt;
  return Status::kRunning;
}

Status NpcBehaviourTree::Idle() {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  hunger_ += kHungerRate * tick_dt;
  core::LogDebug("I'm sleeping");
  return Status::kSuccess;
}

void NpcBehaviourTree::SetupBehaviourTree(Motor* npc_motor, Path* path,
                                          const TileMap* tilemap,
                                          sf::Vector2f cantina_position,
                                          std::vector<Resource> ressources) {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  core::LogDebug("Setup Behaviour Tree");

  hunger_ = 0;

  npc_motor_ = npc_motor;
  path_ = path;
  tilemap_ = tilemap;
  cantina_position_ = cantina_position;
  ressources_ = std::move(ressources);

  sf::Vector2f start = {0, 0};

  auto feedSequence = std::make_unique<Sequence>();
  auto foodPlaceSelection = std::make_unique<Selector>();
  feedSequence->AddChild(
      std::make_unique<Action>([this]() { return CheckHunger(); }));
  feedSequence->AddChild(std::move(foodPlaceSelection));
  feedSequence->AddChild(std::make_unique<Action>([this]() { return Move(); }));
  feedSequence->AddChild(std::make_unique<Action>([this]() { return Eat(); }));

  auto workSequence = std::make_unique<Sequence>();
  workSequence->AddChild(
      std::make_unique<Action>([this]() { return PickRessource(); }));
  workSequence->AddChild(std::make_unique<Action>([this]() { return Move(); }));
  workSequence->AddChild(
      std::make_unique<Action>([this]() { return GetRessource(); }));

  auto selector = std::make_unique<Selector>();
  // Attach the sequence to the selector
  selector->AddChild(std::move(feedSequence));
  selector->AddChild(std::move(workSequence));
  selector->AddChild(std::make_unique<Action>([this]() { return Idle(); }));

  bt_root_ = std::move(selector);
}

void NpcBehaviourTree::Update(float dt) {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  tick_dt = dt;
  bt_root_->Tick();
  // std::cout << "this ? = " << this << "\n";
}
}  // namespace api::ai
