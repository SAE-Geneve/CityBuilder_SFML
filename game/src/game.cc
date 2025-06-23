#include "game.h"

#include "SFML/Graphics.hpp"

#include "ai/npc_manager.h"
#include "graphics/tilemap.h"

namespace game {
    namespace {

        inline sf::Clock clock;

        inline sf::RenderWindow window_;
        inline auto tilemap_ptr_= std::make_unique<TileMap>();
        // inline TileMap tilemap_obj_;
        inline api::ai::NpcManager npc_manager_;

        void Setup(){
            // Create the main window
            window_.create(sf::VideoMode({1280, 1080}), "SFML window");

            tilemap_ptr_->Setup();
            npc_manager_.Add(api::ai::NpcType::kGreen, tilemap_ptr_.get());
            npc_manager_.Add(api::ai::NpcType::kBlue, tilemap_ptr_.get());
            npc_manager_.Add(api::ai::NpcType::kRed, tilemap_ptr_.get());

            // tilemap_obj_.Setup();
            // npc_manager_.Add(api::ai::NpcType::kGreen, &tilemap_obj_);
            // npc_manager_.Add(api::ai::NpcType::kBlue, &tilemap_obj_);
            // npc_manager_.Add(api::ai::NpcType::kRed, &tilemap_obj_);

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
            npc_manager_.Update(dt);

            // Graphic frame
            window_.clear();

            tilemap_ptr_->Draw(window_);
            npc_manager_.Draw(window_);

            window_.display();
        }
    }
}
