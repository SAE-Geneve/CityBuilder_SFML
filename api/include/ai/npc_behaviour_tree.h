//
// Created by sebas on 19/06/2025.
//

#ifndef NPC_BEHAVIOUR_TREE_H
#define NPC_BEHAVIOUR_TREE_H

#include <memory>

#include "ai/bt_node.h"
#include "graphics/tilemap.h"
#include "motion/motor.h"
#include "motion/path.h"
#include "resources/resource.h"
#include "resources/resource_manager.h"

namespace api::ai {
class Npc;
class NpcBehaviourTree {
public:
  NpcBehaviourTree(resource::ResourceManager& resource_manager): resource_manager_(resource_manager) {}
private:
  // Behaviour tree
  std::unique_ptr<core::ai::behaviour_tree::Node> bt_root_;

  // Env informations ----------------------
  const TileMap* tilemap_ = nullptr;
  // Ways of action
  motion::Motor* npc_motor_ = nullptr;
  motion::Path* path_ = nullptr;
  Npc* npc_ = nullptr;

  void set_destination(const sf::Vector2f& destination) const;
  // Actions
  [[nodiscard]] core::ai::behaviour_tree::Status CheckHunger() const;
  [[nodiscard]] core::ai::behaviour_tree::Status Move() const;
  [[nodiscard]] core::ai::behaviour_tree::Status Eat();
  [[nodiscard]] core::ai::behaviour_tree::Status PickResource();
  [[nodiscard]] core::ai::behaviour_tree::Status GetResource();
  [[nodiscard]] core::ai::behaviour_tree::Status Idle();

  // Behaviour Constants
  static constexpr float kHungerRate = 2.f;
  static constexpr float kExploitRate = 1.f;

  // Behaviours
  float hunger_ = 0.0f;
  bool resourceAvailable_ = true;
  float tick_dt = 0;

  sf::Vector2f cantina_position_;
  resource::ResourceManager& resource_manager_;
  resource::Resource* current_ressource_ = nullptr;

 public:
  void SetupBehaviourTree(motion::Motor* npc_motor, motion::Path* path, Npc* npc_,
                          const TileMap* tilemap, sf::Vector2f cantina_position);
  void Update(float dt);
};
}  // namespace api::ai

#endif  // NPC_BEHAVIOUR_TREE_H
