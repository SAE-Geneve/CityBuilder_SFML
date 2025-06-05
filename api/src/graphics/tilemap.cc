//
// Created by sebas on 15/05/2025.
//
#include "graphics/tilemap.h"

#include <iostream>
#include <gtest/internal/gtest-port.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>


TileMap::TileMap(){
}

void TileMap::Setup(){
    textures.Load(files);
    tiles_.fill(Tile::kMaison);
}

void TileMap::Draw(sf::RenderWindow &window){
    int tileIndex = 0;

    sf::Sprite sprite(textures.Get(Tile::kEmpty));

    for (auto element: tiles_) {
        sprite.setTexture(textures.Get(element));
        sprite.setPosition(ScreenPosition(tileIndex));
        window.draw(sprite);

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
