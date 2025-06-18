#include "game.h"

#include "SFML/Graphics.hpp"

#include "ai/npc.h"
#include "ai/npc_manager.h"

namespace game {

    namespace {

        sf::Clock clock;
        sf::RenderWindow window_;
        TileMap<BgTile> background_;
        api::ai::NPCManager npc_manager;

        void Setup(){
            // Create the main window
            window_.create(sf::VideoMode({1280, 1080}), "SFML window");

            background_.Setup(bg_files_);

            //npc_.Setup(&tilemap_);
            npc_manager.Add(api::ai::NpcType::kBlueTruck, &background_);
            npc_manager.Add(api::ai::NpcType::kGreenTruck, &background_);
            npc_manager.Add(api::ai::NpcType::kRedTruck, &background_);

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

            background_.Draw(window_);
            npc_manager.Draw(window_);

            window_.display();
        }
    }
}
