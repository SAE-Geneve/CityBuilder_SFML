#include <optional>
#include "SFML/Graphics.hpp"

#include "game.h"

#include "tilemap.h"
#include "ai/a_star_graph.h"
#include "ai/npc.h"
#include "ai/npc_manager.h"
#include "graphics/camera.h"
#include "graphics/graphic_settings.h"
#include "tiles/world_settings.h"
#include "ui/button_builder.h"
#include "ui/ui_manager.h"

namespace game {
    namespace {


        sf::Clock clock_;
        sf::RenderWindow window_;
        bool isFullscreen_ = false;
        // Size the window/GL context was last (re)created at.
        sf::Vector2u appliedSize_{};

        Tilemap map_;
        api::graphics::Camera camera_;
        api::ai::NPCManager npc_manager_;
        api::ai::AStarGraph astar_graph_(api::tiles::WorldSettings::nb_tiles);

        // UI -----------------------------------------
        api::ui::ui_manager ui_manager;

        void Setup(){

            using namespace api;

            // Create the main window
            window_.create(sf::VideoMode(graphics::window_size_u), "SFML window", sf::State::Fullscreen);
            //isFullscreen_ = true;
            camera_.Setup(graphics::window_size_f, sf::FloatRect({0.f, 0.f}, {tiles::WorldSettings::nb_tiles.x, tiles::WorldSettings::nb_tiles.y}));
            map_.Setup(tiles::WorldSettings::nb_tiles, tiles::WorldSettings::tile_size, astar_graph_);
            npc_manager_.Setup("_assets/kenney_medieval-rts/PNG/Default size/Unit/medievalUnit_01.png", tiles::WorldSettings::nb_tiles);

            for (int i = 0; i < 10000; ++i) {
                npc_manager_.SpawnNPC(astar_graph_);
            }

            ui_manager.InitTexture("_assets/UI/square_buttons_26x26.png");
            ui_manager.InitLabelStyle("_assets/UI/pixelFont-7-8x14-sproutLands.ttf");

            api::ui::ButtonBuilder btnBuilder;

            ui_manager.Register(
                btnBuilder
                .New()
                .WithPosition({50, 50}, {96, 96})
                .WithText("Wood")
                .WithBaseTile(sf::FloatRect({0, 3 * 48}, {48, 48}))
                .WithHoverTile(sf::FloatRect({48, 3 * 48}, {48, 48}))
                .WithClickTile(sf::FloatRect({48, 0 * 48}, {48, 48}))
                .WithClickCallback([]() { std::println("Click Wood"); })
                .WithHoverCallback([]() { std::println("Hover Wood"); })
                .Build()
            );
            ui_manager.Register(
                btnBuilder
                .New()
                .WithPosition({120, 50}, {96, 96})
                .WithText("Rock")
                .WithBaseTile(sf::FloatRect({0, 3 * 48}, {48, 48}))
                .WithHoverTile(sf::FloatRect({48, 3 * 48}, {48, 48}))
                .WithClickTile(sf::FloatRect({48, 0 * 48}, {48, 48}))
                .WithClickCallback([]() { std::println("Click Rock"); })
                .WithHoverCallback([]() { std::println("Hover Rock"); })
                .Build()
            );

            ui_manager.Register(
                btnBuilder
                .WithPosition({190, 50}, {96, 96})
                .WithText("Food")
                .WithClickCallback([]() { std::println("Click Food"); })
                .WithHoverCallback([]() { std::println("Hover Food"); })
                .Build()
            );

            // ui_manager.Register(
            //    btnBuilder
            //    .New()
            //    .WithPosition({0,0},window_size_f)
            //    // .WithClickCallback([](){std::println("Click Background"); })
            //    // .WithHoverCallback([](){std::println("Hover Background, change mouse ?"); })
            //    .Build()
            //    );

        }

        void ToggleFullscreen(){
            isFullscreen_ = !isFullscreen_;
            if (isFullscreen_) {
                window_.create(sf::VideoMode::getDesktopMode(), "SFML window", sf::State::Fullscreen);
            } else {
                window_.create(sf::VideoMode(api::graphics::window_size_u), "SFML window", sf::Style::Default);
            }
            appliedSize_ = window_.getSize();
        }
    } // namespace

    void Loop(){
        Setup();

        // Start the game loop
        while (window_.isOpen()) {
            const float dt = clock_.restart().asSeconds();
            //std::println("New frame ------------------------------------");

            // Process events = Input frame
            int idx_event = 0;
            while (const std::optional event = window_.pollEvent()) {

                //std::println("event treated {}", idx_event++);

                // Close window: exit
                if (event->is<sf::Event::Closed>()) {
                    window_.close();
                }
                if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
                    if (key->code == sf::Keyboard::Key::Enter && key->alt) {
                        ToggleFullscreen();
                        continue;
                    }
                }
                camera_.HandleEvent(*event, window_);
                ui_manager.HandleEvent(*event, window_);
            }
            // Logic frame
            npc_manager_.Update(dt);

            // Rebuild the GL context if the window was resized/maximized.
            camera_.Update(dt);
            // Graphic frame
            window_.clear();

            // Apply the camera view
            camera_.Apply(window_);
            map_.Draw(window_);
            npc_manager_.Draw(window_);

            // Reset the view for UI
            sf::View ui_view_;
            ui_view_.setSize(api::graphics::window_size_f);
            ui_view_.setCenter(api::graphics::window_size_f * 0.5f);
            window_.setView(ui_view_);
            ui_manager.Draw(window_);

            window_.display();
        }
    }
} // namespace game
