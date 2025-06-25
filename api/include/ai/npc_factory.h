//
// Created by sebas on 19/06/2025.
//

#ifndef NPC_FACTORY_H
#define NPC_FACTORY_H
#include "npc_manager.h"

namespace api::ai {
    inline void CreateNpc(std::vector<Npc> &npcs, const NpcType type, const sf::Vector2f start_position, TileMap* tilemap){

        std::cout << "Creating npc" << std::endl;

        switch (type) {
            case NpcType::kBlue:
                npcs.emplace_back();
                npcs.back().Setup("BLUE", "scifiUnit_06.png", tilemap);
                break;
            case NpcType::kRed:
                npcs.emplace_back();
                npcs.back().Setup("RED", "scifiUnit_18.png", tilemap);
                break;
            case NpcType::kGreen:
                npcs.emplace_back();
                npcs.back().Setup("GREEN","scifiUnit_31.png", tilemap);
                break;
            default:
                std::cout << "Error: Npc type not found" << std::endl;
                break;
        }

      if (type == NpcType::kBlue || type == NpcType::kRed || type == NpcType::kGreen) {
          npcs.back().SetPosition(start_position);
      }

    }
}


#endif //NPC_FACTORY_H
