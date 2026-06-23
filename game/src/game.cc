#include <optional>
#include "SFML/Graphics.hpp"
#include "game.h"

#include "tilemap.h"
#include "ai/a_star_graph.h"
#include "ai/npc.h"
#include "ai/npc_manager.h"
#include "graphics/camera.h"
#include "ui/button_builder.h"
#include "ui/ui_manager.h"

namespace game {
    namespace {
        constexpr sf::Vector2i world_size = {1920 * 2, 1080 * 2};
        constexpr sf::Vector2i world_offset = {32, 32};
        constexpr sf::Vector2f window_size_f = {1920.f, 1080.f};
        constexpr sf::Vector2u window_size_u = {1920u, 1080u};

        sf::Clock clock_;
        sf::RenderWindow window_;
        bool isFullscreen_ = false;
        // Size the window/GL context was last (re)created at.
        sf::Vector2u appliedSize_{};

        Tilemap map_;
        api::graphics::Camera camera_;
        api::ai::NPCManager npc_manager_;
        api::ai::AStarGraph astar_graph_(world_size, world_offset);

        // UI -----------------------------------------
        api::ui::ui_manager ui_manager;

        void Setup(){
            // Create the main window
            window_.create(sf::VideoMode(window_size_u), "SFML window", sf::State::Fullscreen);
            //isFullscreen_ = true;
            camera_.Setup(window_size_f, sf::FloatRect({0.f, 0.f}, {world_size.x, world_size.y}));
            map_.Setup(world_size, {world_offset.x, world_offset.y}, astar_graph_);
            npc_manager_.Setup("_assets/kenney_medieval-rts/PNG/Default size/Unit/medievalUnit_01.png", world_size);

            for (int i = 0; i < 500; ++i) {
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

        }

        void ToggleFullscreen(){
            isFullscreen_ = !isFullscreen_;
            if (isFullscreen_) {
                window_.create(sf::VideoMode::getDesktopMode(), "SFML window", sf::State::Fullscreen);
            } else {
                window_.create(sf::VideoMode(window_size_u), "SFML window", sf::Style::Default);
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
            ui_view_.setSize(window_size_f);
            ui_view_.setCenter(window_size_f * 0.5f);
            window_.setView(ui_view_);
            ui_manager.Draw(window_);

            window_.display();
        }
    }
} // namespace game
