#include <optional>
#include "SFML/Graphics.hpp"
#include "game.h"
#include "graphics/tilemap_renderer.h"
#include "graphics/tilesheet.h"

namespace game {
    namespace {
        // [[maybe_unused]] sf::Clock clock;
        sf::RenderWindow window_;


        graphics::TilemapRenderer tilemap_;
        graphics::Tilesheet<BackgroundTiles> tilesheet_;

        void Setup(){
            // Create the main window
            window_.create(sf::VideoMode({1920, 1080}), "SFML window");

            if (tilesheet_.InitTileSheet("_assets/tiles/RTS_medieval@2_no_margins.png", 128)) {
                tilesheet_.AddTile(BackgroundTiles::kGrassA, 0, 0);
                tilesheet_.AddTile(BackgroundTiles::kGrassB, 1, 0);
                tilesheet_.AddTile(BackgroundTiles::kWaterA, 0, 2);
                tilesheet_.AddTile(BackgroundTiles::kWaterB, 1, 2);

                tilemap_.Setup(tilesheet_.GetTexture(), {1920, 1080}, {64, 64},
                               tilesheet_.GetBounds(BackgroundTiles::kGrassA));
            }
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
