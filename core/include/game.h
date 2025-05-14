//
// Created by sebas on 14/05/2025.
//

#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

class Game {

public:
    Game();
    void Loop();

private:
    sf::RenderWindow window_;
    sf::Texture grass_texture_;
};

#endif //GAME_H
