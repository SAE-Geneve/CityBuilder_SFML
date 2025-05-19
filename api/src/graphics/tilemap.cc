//
// Created by sebas on 15/05/2025.
//
#include "graphics/tilemap.h"

#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

void TileMap::Draw(sf::RenderWindow &window) const{
    int tileIndex = 0;

    sf::Sprite sprite(default_texture_);

    for (auto element: tiles_) {

        if (element != Tile::EMPTY) {
            switch (element) {
                case Tile::GRASS:
                    sprite.setTexture(grass_texture_);
                    break;
                default:
                    break;
            }

            sprite.setPosition(ScreenPosition(tileIndex));
            window.draw(sprite);
        }

        tileIndex++;
    }
}

sf::Vector2f TileMap::ScreenPosition(const int index){
    float x = ceil((index % (kWidth / kPixelStep)) * kPixelStep);
    float y = ceil((index / (kWidth / kPixelStep)) * kPixelStep);

    //std::cout << "Calculated Screen Position : " << x << "." << y << "\n";

    return {x, y};
}

int TileMap::Index(const sf::Vector2f screenPosition){
    return static_cast<int>(ceil(screenPosition.y / kPixelStep * kWidth)) +
           static_cast<int>(ceil(screenPosition.x / kPixelStep));
}

void TileMap::Setup(){
    grass_texture_.loadFromFile("_assets/sprites/grass.png");
    default_texture_.loadFromFile("_assets/sprites/empty.png");

    tiles_.fill(Tile::GRASS);
}
