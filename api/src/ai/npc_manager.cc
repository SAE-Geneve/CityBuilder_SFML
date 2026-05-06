//
// Created by sebas on 19/06/2025.
//

#include "ai/npc_manager.h"

#include "ai/npc_factory.h"
#include "profiling/profiling.h"

namespace api::ai {

void NpcManager::Add(NpcType type, sf::Vector2f start_position,
                     TileMap* tilemap, ResourceManager& resource_manager) {
  PROFILE_ZONE();
  CreateNpc(npcs_, type, start_position, tilemap, resource_manager);
}

void NpcManager::Update(float dt) {
  PROFILE_ZONE();
  for (auto& npc : npcs_) {
    npc.Update(dt);
  }
}

void NpcManager::Draw(sf::RenderWindow& window) {
  PROFILE_ZONE();
  for (auto& npc : npcs_) {
    npc.Draw(window);
  }
}

}  // namespace api::ai
