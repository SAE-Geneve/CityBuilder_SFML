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

void NpcBehaviourTree::SetDestination(const sf::Vector2f& destination) const {

	Path path = Astar::GetPath(TileMap::GetStep(), npc_motor_->GetPosition(), destination,
							   this->tilemap_->GetWalkables());
	if (path.IsValid()) {
		this->path_->Fill(path.Points());
		this->npc_motor_->SetDestination(path.StartPoint());
	}

}

Status NpcBehaviourTree::CheckHunger() const {
	// std::cout << "this ? = " << this << "\n";
	// std::cout << "Am I hungry ? " << std::to_string(hunger_);

	if (hunger_ >= 100) {
		// std::cout << " : Yes, I need to find food\n";

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

		SetDestination(cantina_position_);

		return Status::kSuccess;

	} else {
		// std::cout << " : No, I can wait\n";
		return Status::kFailure;
	}
}

Status NpcBehaviourTree::Move() const {
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
	// No failure, until we have food storage system
	hunger_ -= kHungerRate * tick_dt;
	if (hunger_ > 0) {
		return Status::kRunning;
	} else {
		return Status::kSuccess;
	}
}

Status NpcBehaviourTree::PickRessource() {

	if (ressources_.empty()) {
		std::cout << "No ressources available\n";
		return Status::kFailure;
	}

	std::mt19937 gen{std::random_device{}()};
	std::uniform_int_distribution<size_t> dist(0, ressources_.size() - 1);
	
	if (ressources_[dist(gen)].GetQty() > 0) {
		current_ressource_ = ressources_[dist(gen)];
		SetDestination(TileMap::ScreenPosition(current_ressource_.GetTileIndex()));

		if (path_->IsValid())
			return Status::kSuccess;

	}

	return Status::kFailure;


}

Status NpcBehaviourTree::GetRessource() {
	if (current_ressource_.GetQty() <= 0) {
		return Status::kSuccess;
	}

	current_ressource_.Exploit(kExploitRate * tick_dt);
	hunger_ += kHungerRate * tick_dt;
	return Status::kRunning;
}

Status NpcBehaviourTree::Idle() {
	hunger_ += kHungerRate * tick_dt;
	std::cout << "I'm sleeping" << "\n";
	return Status::kSuccess;
}

void NpcBehaviourTree::SetupBehaviourTree(Motor* npc_motor, Path* path,
										  TileMap* tilemap,
										  sf::Vector2f cantina_position,
										  std::vector<Ressource> ressources) {
	std::cout << "Setup Behaviour Tree\n";

	hunger_ = 0;

	npc_motor_ = npc_motor;
	path_ = path;
	tilemap_ = tilemap;
	cantina_position_ = cantina_position;
	ressources_ = ressources;

	sf::Vector2f start = {0, 0};

	auto feedSequence = std::make_unique<Sequence>();
	feedSequence->AddChild(
		std::make_unique<Action>([this]() { return CheckHunger(); }));
	feedSequence->AddChild(
		std::make_unique<Action>([this]() { return Move(); }));
	feedSequence->AddChild(
		std::make_unique<Action>([this]() { return Eat(); }));

	auto workSequence = std::make_unique<Sequence>();
	workSequence->AddChild(
		std::make_unique<Action>([this]() { return PickRessource(); }));
	workSequence->AddChild(
		std::make_unique<Action>([this]() { return Move(); }));
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
	tick_dt = dt;
	bt_root_->Tick();
	// std::cout << "this ? = " << this << "\n";
}
}  // namespace api::ai
