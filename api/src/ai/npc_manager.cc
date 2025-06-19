//
// Created by sebas on 19/06/2025.
//

#include "ai/npc_manager.h"

namespace api::ai {

    void NpcManager::Add(const TileMap* tilemap){
        npcs_.emplace_back();
        npcs_.back().Setup(tilemap);
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