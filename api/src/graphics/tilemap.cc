//
// Created by sebas on 15/05/2025.
//
#include "graphics/tilemap.h"

#include <iostream>
#include <random>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

TileMap::TileMap(){
}

void TileMap::Setup(){
    textures.Load(files);

    // Créer un générateur de nombres aléatoires
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 5);

    // Remplir le tableau avec de l'herbe ou de l'eau de manière aléatoire
    for (int tileIndex = 0; tileIndex < tiles_.size(); ++tileIndex) {
        if (dis(gen) == 0) {
            tiles_[tileIndex] = Tile::kWater;
        }else {
            tiles_[tileIndex] = Tile::kGrass16;
            walkables_.push_back(ScreenPosition(tileIndex));

        // std::cout << "add walkable " << tileIndex << " : [" << ScreenPosition(tileIndex).x << " : " << ScreenPosition(tileIndex).y << "]\n";

        }
    }
}

void TileMap::Draw(sf::RenderWindow &window){
    int tileIndex = 0;

    sf::Sprite sprite(textures.Get(Tile::kEmpty));

    for (auto tile: tiles_) {
        sprite.setTexture(textures.Get(tile));
        sprite.setPosition(ScreenPosition(tileIndex));
        window.draw(sprite);

        tileIndex++;
    }
}

std::vector<sf::Vector2f> TileMap::GetWalkables() const{
    return walkables_;
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
