#include <optional>
#include "SFML/Graphics.hpp"
#include "game.h"
#include "graphics/tilemap_renderer.h"
#include "graphics/tilesheet.h"

namespace game {
    namespace {
        sf::Clock clock;
        sf::RenderWindow window_;

        sf::Texture tilesheet;
        graphics::TilemapRenderer tilemap_;

        void Setup() {
            // Create the main window
            window_.create(sf::VideoMode({1920, 1080}), "SFML window");

            tilesheet.loadFromFile("_assets/tiles/RTS_medieval@2_no_margins.png");

            tilemap_.Setup(&tilesheet, {1920, 1080}, {64, 64}, graphics::tilesheet::ConstructRect(0,0,128,128));
        }
    } // namespace

    void Loop() {
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

            // Graphic frame
            window_.clear();
            tilemap_.Draw(window_);
            window_.display();
        }
    }
} // namespace game
