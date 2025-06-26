//
// Created by sebas on 19/06/2025.
//

#include "ai/npc_manager.h"

#include "ai/npc_factory.h"

namespace api::ai {

void NpcManager::Add(NpcType type, sf::Vector2f start_position, TileMap* tilemap, RessourceManager& ressource_manager) {
	CreateNpc(npcs_, type, start_position, tilemap, ressource_manager);
}

void NpcManager::Update(float dt) {
  for (auto& npc : npcs_) {
    npc.Update(dt);
  }
}

void NpcManager::Draw(sf::RenderWindow& window) {
  for (auto& npc : npcs_) {
    npc.Draw(window);
  }
}

}  // namespace api::ai
