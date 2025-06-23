//
// Created by sebas on 19/06/2025.
//

#include "ai/npc_behaviour_tree.h"

#include <functional>
#include <iostream>
#include <random>

#include "ai/bt_action.h"
#include "ai/bt_selector.h"
#include "ai/bt_sequence.h"
#include "motion/AStar.h"
#include "motion/path.h"

using namespace core::ai::behaviour_tree;
using namespace api::motion;

namespace api::ai {

void NpcBehaviourTree::SetRandomDestination() const {
  static std::mt19937 gen{std::random_device{}()};
  static std::uniform_int_distribution<size_t> dist(
      0, this->tilemap_->GetWalkables().size() - 1);

  sf::Vector2f end = this->tilemap_->GetWalkables().at(dist(gen));

  Path path = Astar::GetPath(64, npc_motor_->GetPosition(), end,
                             this->tilemap_->GetWalkables());
  this->path_->Fill(path.Points());
  this->npc_motor_->SetDestination(path.StartPoint());
}

Status NpcBehaviourTree::CheckHunger() const {
  std::cout << "this ? = " << this << "\n";
  std::cout << "Am I hungry ? " << std::to_string(hunger_);

  if (hunger_ >= 100) {
    std::cout << " : Yes, I need to find food\n";

    if (!tilemap_) {
      std::cout << "No tilemap\n";
      return Status::kFailure;
    }
    if (!path_) {
      std::cout << "No path\n";
      return Status::kFailure;
    }
    if (!npc_motor_) {
      std::cout << "No motor\n";
      return Status::kFailure;
    }

    SetRandomDestination();

    return Status::kSuccess;

  } else {
    std::cout << " : No, I can wait\n";
    return Status::kFailure;
  }
}

Status NpcBehaviourTree::Move() const {
  // if destination not reachable, return failure
  if (!path_->IsValid()) {
    std::cout << "Not reachable" << path_->IsValid() << "\n";
    return Status::kFailure;
  } else {
    std::cout << "I'm moving" << "\n";
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
  // No failure, until we have food storage system
  hunger_ -= kHungerRate;
  if (hunger_ > 0) {
    return Status::kRunning;
  } else {
    return Status::kSuccess;
  }
}

Status NpcBehaviourTree::Work() {
  hunger_ += kHungerRate * 5;
  if (resourceAvailable_) {
    std::cout << "Resource Available, working....." << "\n";
    return Status::kSuccess;
  }
  return Status::kFailure;
}

Status NpcBehaviourTree::Idle() {
  hunger_ += kHungerRate * 5;
  std::cout << "I'm sleeping" << "\n";
  return Status::kSuccess;
}

void NpcBehaviourTree::SetupBehaviourTree(Motor* npc_motor, Path* path,
                                          TileMap* tilemap) {
  std::cout << "Setup Behaviour Tree\n";

  hunger_ = 0;

  npc_motor_ = npc_motor;
  path_ = path;
  tilemap_ = tilemap;

  auto feedSequence = std::make_unique<Sequence>();
  feedSequence->AddChild(
      std::make_unique<Action>([this]() { return CheckHunger(); }));
  feedSequence->AddChild(std::make_unique<Action>([this]() { return Move(); }));
  feedSequence->AddChild(std::make_unique<Action>([this]() { return Eat(); }));

  auto selector = std::make_unique<Selector>();
  // Attach the sequence to the selector
  selector->AddChild(std::move(feedSequence));
  selector->AddChild(std::make_unique<Action>([this]() { return Work(); }));
  selector->AddChild(std::make_unique<Action>([this]() { return Idle(); }));

  bt_root_ = std::move(selector);
}

void NpcBehaviourTree::Update(float dt) {
  bt_root_->Tick();
  std::cout << "this ? = " << this << "\n";
  /*
    std::cout << "Check hunger\n";
    CheckHunger();
    */// npc_motor_->Update(dt);
}
}  // namespace api::ai
