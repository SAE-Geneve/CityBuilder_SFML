//
// Created by sebas on 19/06/2025.
//

#ifndef NPC_FACTORY_H
#define NPC_FACTORY_H
#include <iostream>

#include "npc_manager.h"

namespace api::ai {
    inline void CreateNpc(std::vector<Npc> &npcs, const NpcType type, const sf::Vector2f& start_position, const TileMap* tilemap, const ResourceManager& resource_manager){

        std::cout << "Creating npc" << std::endl;

        switch (type) {
            case NpcType::kBlueWood:
                npcs.emplace_back();
                npcs.back().Setup("BLUE", "scifiUnit_06.png", tilemap, start_position, resource_manager.GetResources(Resource::Type::kWood));
                break;
            case NpcType::kRedRock:
                npcs.emplace_back();
				npcs.back().Setup(
					"RED", "scifiUnit_18.png", tilemap, start_position,
					resource_manager.GetResources(Resource::Type::kStone));
                break;
            case NpcType::kGreenFood:
                npcs.emplace_back();
				npcs.back().Setup(
					"GREEN", "scifiUnit_31.png", tilemap, start_position,
					resource_manager.GetResources(Resource::Type::kFood));
                break;
            default:
                std::cout << "Error: Npc type not found" << std::endl;
                break;
        }

      if (type == NpcType::kBlueWood || type == NpcType::kRedRock || type == NpcType::kGreenFood) {
          npcs.back().SetPosition(start_position);
      }

    }
}


#endif //NPC_FACTORY_H
