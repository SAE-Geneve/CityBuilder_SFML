//
// Created by sebas on 17/06/2025.
//

#include "ai/npc_manager.h"

namespace api::ai {

void NPCManager::Add(NpcType type, const TileMap* tileMap) {

  std::cout << "Adding npc" << std::endl;

  npcs_.push_back(std::make_unique<Npc>());
  npcs_.back()->Setup(tileMap);

}

void NPCManager::Update(float dt) {
    for (const auto& npc : npcs_) {
      npc->Update(dt);
    }
}

void NPCManager::Draw(sf::RenderWindow &window) {
  for (const auto& npc : npcs_) {
    npc->Draw(window);
  }
}

}

