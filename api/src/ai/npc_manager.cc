//
// Created by sebas on 10.06.2026.
//

#include "ai/npc_manager.h"

namespace api::ai{
    void NPCManager::Setup(std::string_view sprite_path, sf::Vector2f world_size){

    world_size_ = world_size;
    sprite_path_ = sprite_path;
    if (texture_->loadFromFile(std::string(sprite_path))) {

    }

}

void NPCManager::Update(float dt){
    for (auto &npc : npcs_){
        npc.Update(dt);
    }
}

void NPCManager::Draw(sf::RenderWindow &window){
    for (auto &npc : npcs_){
        npc.Draw(window);
    }
}

void NPCManager::SpawnNPC(){
    npcs_.emplace_back();
    npcs_.back().Setup(texture_, world_size_, )
}
}
