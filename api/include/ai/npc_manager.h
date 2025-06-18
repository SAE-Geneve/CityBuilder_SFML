//
// Created by sebas on 17/06/2025.
//

#ifndef NPC_MANAGER_H
#define NPC_MANAGER_H

#include "npc.h"
#include "npc_factory.h"

namespace api::ai {

  class NPCManager {

  private:
    std::vector<std::unique_ptr<Npc>> npcs_;

  public:
    void Add(NpcType type, const TileMap<BgTile>* tileMap);
    void Update(float dt);
    void Draw(sf::RenderWindow &window);

  };

}

#endif //NPC_MANAGER_H
