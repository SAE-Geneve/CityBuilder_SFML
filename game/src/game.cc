#include <optional>
#include "SFML/Graphics.hpp"
#include "game.h"

#include "tilemap.h"
#include "graphics/camera.h"
#include "graphics/tilemap_renderer.h"
#include "graphics/tilesheet.h"

namespace game {
    namespace {
        constexpr sf::Vector2f world_size = {1920.f * 5, 1080.f * 5};
        constexpr sf::Vector2f window_size_f = {1920.f, 1080.f};
        constexpr sf::Vector2u window_size_u = {1920u, 1080u};

        sf::Clock clock_;
        sf::RenderWindow window_;

        Tilemap tilemap_;
        graphics::Camera camera_;

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
            const float dt = clock_.restart().asSeconds();

            // Process events = Input frame
            while (const std::optional event = window_.pollEvent()) {
                // Close window: exit
                if (event->is<sf::Event::Closed>()) {
                    window_.close();
                }
                camera_.HandleEvent(*event, window_);
            }

            camera_.Update(dt);
            camera_.Apply(window_);

            // Graphic frame
            window_.clear();
            tilemap_.Draw(window_);
            window_.display();
        }
    }
} // namespace game
