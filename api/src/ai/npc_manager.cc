//
// Created by sebas on 10.06.2026.
//

#include "ai/npc_manager.h"

#include <mdspan>

#include "ai/a_star_graph.h"
#include "rng/rng.h"

namespace api::ai {
    void NPCManager::Setup(std::string_view sprite_path, sf::Vector2i world_size){
        world_size_ = world_size;
        sprite_path_ = sprite_path;
        if (texture_->loadFromFile(std::string(sprite_path))) {
            texture_bounds_ = sf::FloatRect(
                sf::Vector2f(0, 0),
                sf::Vector2f(texture_->getSize())
            );
        }
        renderer_.SetTexture(texture_.get());
    }

    void NPCManager::Update(float dt){
        renderer_.ClearVertices();

        for (auto &npc: npcs_) {
            npc->Update(dt);
            renderer_.AddTile(npc->Position() - sf::Vector2f{16, 16}, {64, 64}, texture_bounds_);
        }
    }

    void NPCManager::Draw(sf::RenderWindow &window){
        renderer_.Draw(window);
        // for (auto &npc: npcs_) {
        //     npc->Draw(window);
        // }
    }

    void NPCManager::SpawnNPC(AStarGraph &graph){
        npcs_.emplace_back(std::make_unique<Npc>());
        npcs_.back()->Setup(texture_.get(), world_size_, graph.GetRandomNode(), graph);
    }
}
