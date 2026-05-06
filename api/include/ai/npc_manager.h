//
// Created by sebas on 19/06/2025.
//

#ifndef NPC_MANAGER_H
#define NPC_MANAGER_H

#include "ai/npc.h"
#include "resources/resource_manager.h"

namespace api::ai {



class NpcManager {

  std::vector<Npc> npcs_;

 public:
  void Add(NpcType type, sf::Vector2f start_position, TileMap* tilemap, ResourceManager& resource_manager);
  void Update(float dt);
  void Draw(sf::RenderWindow& window);

  [[nodiscard]] std::ptrdiff_t Count() const noexcept { return std::ssize(npcs_); }
};
}  // namespace api::ai

#endif  // NPC_MANAGER_H
