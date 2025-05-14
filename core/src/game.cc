#include "game.h"

 Game::Game() {
     // Create the main window
     window_.create(sf::VideoMode({1280, 1080}), "SFML window");
     // Load a sprite to display
     grass_texture_.loadFromFile("assets/sprites/grass.png");

}

void Game::Loop(){

     const sf::Sprite sprite(grass_texture_);

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

         window_.draw(sprite);

         window_.display();

     }

}

