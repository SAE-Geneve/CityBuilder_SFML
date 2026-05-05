#include "game.h"

#include "SFML/Graphics.hpp"

namespace game {
namespace {
sf::Clock clock;
sf::RenderWindow window_;

void Setup() {
  // Create the main window
  window_.create(sf::VideoMode({1280, 1080}), "SFML window");
}
}  // namespace

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
    window_.display();
  }
}
}  // namespace game
