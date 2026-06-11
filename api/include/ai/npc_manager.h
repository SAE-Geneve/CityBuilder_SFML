//
// Created by sebas on 10.06.2026.
//

#ifndef CITYBUILDER_NPC_MANAGER_H
#define CITYBUILDER_NPC_MANAGER_H
#include <SFML/Graphics/RenderWindow.hpp>
#include <string_view>

#include "npc.h"
namespace api::ai{
    class NPCManager {

    std::vector<api::ai::Npc> npcs_;
    sf::Vector2f world_size_;
    std::string sprite_path_;
    std::unique_ptr<sf::Texture> texture_ = std::make_unique<sf::Texture>();
    public:
    void Setup(std::string_view sprite_path, sf::Vector2f world_size);
    void Update(float dt);
    void Draw(sf::RenderWindow &window);
    void SpawnNPC();
};
}

#endif //CITYBUILDER_NPC_MANAGER_H
