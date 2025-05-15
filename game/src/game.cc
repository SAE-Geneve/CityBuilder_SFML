#include "game.h"

#include "SFML/Graphics.hpp"

#include "map_generator.h"

namespace game {

  namespace {



    sf::RenderWindow window_;

    MapGenerator map_generator_;

    void Setup() {
      // Create the main window
      window_.create(sf::VideoMode({1280, 1080}), "SFML window");
      // Load a sprite to display
    }

  }

  void Loop(){

    Setup();



      // Start the game loop
      while (window_.isOpen()) {

        // Process events
        while (const std::optional event = window_.pollEvent())
        {
          // Close window: exit
          if (event->is<sf::Event::Closed>()) {
            window_.close();
          }
        }

        window_.clear();

        window_.display();

      }

    }

}




