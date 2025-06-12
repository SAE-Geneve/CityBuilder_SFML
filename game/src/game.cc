#include "game.h"

#include "SFML/Graphics.hpp"

#include "ai/npc.h"
#include "graphics/tilemap.h"

namespace game {
    namespace {
        sf::Clock clock;

        sf::RenderWindow window_;
        TileMap tilemap_;

        api::ai::Npc npc_;

        void Setup(){
            // Create the main window
            window_.create(sf::VideoMode({1280, 1080}), "SFML window");

            tilemap_.Setup();

            npc_.Setup();
            Path temp_path_;
            std::vector<sf::Vector2f> points;
            points.push_back(sf::Vector2f(10, 10));
            points.push_back(sf::Vector2f(100, 100));
            points.push_back(sf::Vector2f(150, 150));
            points.push_back(sf::Vector2f(200, 200));

            temp_path_.Fill(points);

            npc_.SetPath(temp_path_);

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

            npc_.Update(dt);

            // Graphic frame
            window_.clear();

            tilemap_.Draw(window_);
            npc_.Draw(window_);

            window_.display();
        }
    }
}
