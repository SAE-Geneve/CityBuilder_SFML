//
// Created by sebas on 17/06/2025.
//

#ifndef NPC_FACTORY_H
#define NPC_FACTORY_H

#include <memory>
#include "npc.h"

namespace api::ai
{

enum class NpcType {
  kBlueTruck,
  kRedTruck,
  kGreenTruck
};

class NPCFactory {
public:

  // Méthode de création qui inclut le setup
  static std::unique_ptr<Npc> createNPC(NpcType type, const TileMap<BgTile>* tileMap) {
    std::unique_ptr<Npc> npc = std::make_unique<Npc>();

    std::array<std::string_view, static_cast<size_t>(Npc::Sprites::kLength)> files;
    switch (type) {
      case NpcType::kBlueTruck:
        files = {"empty.png", "scifiUnit_06.png"};
        break;
      case NpcType::kRedTruck:
        files = {"empty.png", "scifiUnit_19.png"};
        break;
      case NpcType::kGreenTruck:
        files = {"empty.png", "scifiUnit_32.png"};
        break;
      default:
        break;
        // ... autres types
    }

    // Configuration commune à tous les PNJ
    if (npc) {
      npc->Setup(tileMap, files);
    }

    return npc;
  }
};
}



#endif //NPC_FACTORY_H
