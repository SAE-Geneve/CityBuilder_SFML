//
// Created by sebas on 19/06/2025.
//

#ifndef NPC_FACTORY_H
#define NPC_FACTORY_H
#include "npc_manager.h"

namespace api::ai {
    inline void CreateNpc(std::vector<Npc> &npcs, const NpcType type, sf::Vector2f& start_position, TileMap* tilemap, RessourceManager& ressource_manager){

        std::cout << "Creating npc" << std::endl;

        switch (type) {
            case NpcType::kBlueWood:
                npcs.emplace_back();
                npcs.back().Setup("BLUE", "scifiUnit_06.png", tilemap, start_position, ressource_manager.GetRessources(Ressource::Type::kWood));
                break;
            case NpcType::kRedRock:
                npcs.emplace_back();
				npcs.back().Setup(
					"RED", "scifiUnit_18.png", tilemap, start_position,
					ressource_manager.GetRessources(Ressource::Type::kStone));
                break;
            case NpcType::kGreenFood:
                npcs.emplace_back();
				npcs.back().Setup(
					"GREEN", "scifiUnit_31.png", tilemap, start_position,
					ressource_manager.GetRessources(Ressource::Type::kFood));
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
