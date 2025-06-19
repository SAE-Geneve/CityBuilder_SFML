//
// Created by sebas on 19/06/2025.
//

#ifndef NPC_FACTORY_H
#define NPC_FACTORY_H
#include "npc_manager.h"

namespace api::ai {
    inline void CreateNpc(std::vector<Npc> &npcs, NpcType type, const TileMap *tilemap){
        npcs.emplace_back();

        switch (type) {
            case NpcType::kBlue:
                npcs.back().Setup("scifiUnit_06.png", tilemap);
                break;
            case NpcType::kRed:
                npcs.back().Setup("scifiUnit_18.png", tilemap);
                break;
            case NpcType::kGreen:
                npcs.back().Setup("scifiUnit_31.png", tilemap);
                break;
            default:
                break;
        }
    }
}


#endif //NPC_FACTORY_H
