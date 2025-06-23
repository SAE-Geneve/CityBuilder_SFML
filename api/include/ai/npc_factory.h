//
// Created by sebas on 19/06/2025.
//

#ifndef NPC_FACTORY_H
#define NPC_FACTORY_H
#include "npc_manager.h"

namespace api::ai {
    inline void CreateNpc(std::vector<Npc> &npcs, const NpcType type, TileMap* tilemap){


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
                break;
        }
    }
}


#endif //NPC_FACTORY_H
