#include <optional>
#include "SFML/Graphics.hpp"
#include "game.h"

#include "tilemap.h"
#include "ai/a_star_graph.h"
#include "ai/npc.h"
#include "ai/npc_manager.h"
#include "graphics/camera.h"

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

        void Setup(){
            // Create the main window
            window_.create(sf::VideoMode(window_size_u), "SFML window", sf::State::Fullscreen);
            //isFullscreen_ = true;
            camera_.Setup(window_size_f, sf::FloatRect({0.f, 0.f}, {world_size.x, world_size.y}));
            map_.Setup(world_size, {world_offset.x, world_offset.y}, astar_graph_);
            npc_manager_.Setup("_assets/kenney_medieval-rts/PNG/Default size/Unit/medievalUnit_01.png", world_size);

            for (int i = 0; i < 100; ++i) {
                npc_manager_.SpawnNPC(astar_graph_);
            }
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

            // Process events = Input frame
            while (const std::optional event = window_.pollEvent()) {
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
            }

            // Rebuild the GL context if the window was resized/maximized.
            camera_.Update(dt);
            camera_.Apply(window_);

            // Logic frame
            npc_manager_.Update(dt);

            // Graphic frame
            window_.clear();
            map_.Draw(window_);
            npc_manager_.Draw(window_);
            window_.display();
        }
    }
} // namespace game
