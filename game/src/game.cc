#include <optional>
#include "SFML/Graphics.hpp"
#include "game.h"

#include "tilemap.h"

namespace game {
    namespace {
        // [[maybe_unused]] sf::Clock clock;
        sf::RenderWindow window_;
        Tilemap tilemap_;

        void Setup(){
            // Create the main window
            window_.create(sf::VideoMode({1920, 1080}), "SFML window");
            tilemap_.Setup({1920,1080}, {32, 32});
        }
    } // namespace

    void Loop(){

        Setup();

        // Start the game loop
        while (window_.isOpen()) {
            //auto dt = clock.restart().asSeconds();

            // Process events = Input frame
            while (const std::optional event = window_.pollEvent()) {
                // Close window: exit
                if (event->is<sf::Event::Closed>()) {
                    window_.close();
                }
            }

            // Graphic frame
            window_.clear();
            tilemap_.Draw(window_);
            window_.display();
        }
    }
} // namespace game
