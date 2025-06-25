#include "game.h"

#include "SFML/Graphics.hpp"

#include "ai/npc_manager.h"
#include "graphics/tilemap.h"
#include "ui/clickable.h"

namespace game {
    namespace {

        inline sf::Clock clock;

        inline sf::RenderWindow window_;
        inline auto tilemap_ptr_= std::make_unique<TileMap>();
        // inline TileMap tilemap_obj_;
        inline api::ai::NpcManager npc_manager_;

        inline api::ui::Clickable clickable_;

        inline sf::RectangleShape rect_;

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

            rect_.setPosition({static_cast<float>(window_.getSize().x / 2), static_cast<float>(window_.getSize().y / 2)});
            rect_.setSize({200, 200});

            clickable_.SetZone(sf::IntRect(
                {static_cast<int>(window_.getSize().x / 2), static_cast<int>(window_.getSize().y / 2)},
                {200, 200})
                );
            clickable_.OnReleasedLeft = [] () {std::cout << "Left Released" << std::endl;};
            clickable_.OnReleasedRight = [] () {std::cout << "Right Released" << std::endl;};
            clickable_.OnPressedLeft = [] () {std::cout << "Left Pressed" << std::endl;};
            clickable_.OnPressedRight = [] () {std::cout << "Right Pressed" << std::endl;};
            clickable_.OnHoverEnter = [] () {std::cout << "Hover Enter" << std::endl;};
            clickable_.OnHoverExit = [] () {std::cout << "Hover Exit" << std::endl;};

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

                clickable_.HandleEvent(event);

            }

            // GamePlay, physic frame
            npc_manager_.Update(dt);

            // Graphic frame
            window_.clear();

            tilemap_ptr_->Draw(window_);
            npc_manager_.Draw(window_);

            window_.draw(rect_);

            window_.display();
        }
    }
}
