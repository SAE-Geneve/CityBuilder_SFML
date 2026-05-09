#ifndef API_AI_NPC_FACTORY_H_
#define API_AI_NPC_FACTORY_H_

#include "npc_manager.h"
#include "profiling/profiling.h"
#include "utils/log.h"

namespace api::ai {
inline void CreateNpc(std::vector<Npc>& npcs, NpcType type,
                      sf::Vector2f start_position, const TileMap* tilemap,
                      resource::ResourceManager& resource_manager) {
  PROFILE_ZONE();

  switch (type) {
    case NpcType::kBlueWood:
      npcs.emplace_back();
      npcs.back().Setup(type, "scifiUnit_06.png", tilemap, start_position,
                        resource_manager);
      break;
    case NpcType::kRedRock:
      npcs.emplace_back();
      npcs.back().Setup(type, "scifiUnit_18.png", tilemap, start_position,
                        resource_manager);
      break;
    case NpcType::kGreenFood:
      npcs.emplace_back();
      npcs.back().Setup(type, "scifiUnit_31.png", tilemap, start_position,
                        resource_manager);
      break;
    default:
      core::LogError("Error: Npc type not found");
      throw std::runtime_error("Error: Npc type not found");
  }

  npcs.back().set_position(start_position);

}
}  // namespace api::ai

#endif  // API_AI_NPC_FACTORY_H_
