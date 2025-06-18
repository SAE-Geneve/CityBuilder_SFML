//
// Created by sebas on 17/06/2025.
//

#include "ai/npc_manager.h"

namespace api::ai {

void NPCManager::Add(NpcType type, const TileMap* tileMap) {

  std::cout << "Adding npc" << std::endl;

  // std::string_view files[static_cast<size_t>(Npc::Sprites::kLength)] = {"empty.png", "scifiUnit_19.png"};
  // npcs_.push_back(std::make_unique<Npc>());
  // npcs_.back()->Setup(tileMap, files);

  npcs_.push_back(NPCFactory::createNPC(type, tileMap));

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

