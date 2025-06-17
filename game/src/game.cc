#include "game.h"

#include "SFML/Graphics.hpp"

#include "ai/npc.h"
#include "ai/npc_manager.h"
#include "graphics/tilemap.h"

namespace game {
    namespace {
        sf::Clock clock;

        sf::RenderWindow window_;
        TileMap tilemap_;

        //api::ai::Npc npc_;
        api::ai::NPCManager npc_manager;

        void Setup(){
            // Create the main window
            window_.create(sf::VideoMode({1280, 1080}), "SFML window");

            tilemap_.Setup();

            //npc_.Setup(&tilemap_);
            npc_manager.Add(api::ai::NpcType::kBlueTruck, &tilemap_);

        }
    }

    void Loop(){
        Setup();

        // Start the game loop
        while (window_.isOpen()) {

            auto dt = clock.restart().asSeconds();

            // Process events = Input frame
            while (const std::optional event = window_.pollEvent()) {
                // Close window: exit
                if (event->is<sf::Event::Closed>()) {
                    window_.close();
                }
            }

            // GamePlay, physic frame
            npc_manager.Update(dt);

            // Graphic frame
            window_.clear();

            tilemap_.Draw(window_);
            npc_manager.Draw(window_);

            window_.display();
        }
    }
}
