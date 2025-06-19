//
// Created by sebas on 19/06/2025.
//

#include "ai/npc_manager.h"
#include "ai/npc_factory.h"

namespace api::ai {

    void NpcManager::Add(NpcType type, const TileMap* tilemap){
        CreateNpc(npcs_, type, tilemap);
    }

    void NpcManager::Update(float dt){
        for (auto& npc : npcs_) {
            npc.Update(dt);
        }
    }

    void NpcManager::Draw(sf::RenderWindow &window){
        for (auto& npc : npcs_) {
            npc.Draw(window);
        }
    }

}
