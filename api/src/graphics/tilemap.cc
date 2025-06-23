//
// Created by sebas on 15/05/2025.
//
#include "graphics/tilemap.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>
#include <random>

#include "noise/FastNoiseLite.h"

void TileMap::Setup() {
  // Create and configure FastNoise object
  FastNoiseLite noise;
  noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
  noise.SetSeed(1309);
  noise.SetFrequency(0.01f);

  textures.Load(files);

  // Remplir le tableau avec de l'herbe ou de l'eau de manière aléatoire
  for (int tileIndex = 0; tileIndex < tiles_.size(); ++tileIndex) {
    sf::Vector2f pos = ScreenPosition(tileIndex);

    auto value = 0.5f * (noise.GetNoise(pos.x, pos.y) + 1);

    std::cout << "noise value = " << value << "\n";

    if (value < 0.35f) {
      tiles_[tileIndex] = Tile::kWater;
    } else {
      tiles_[tileIndex] = Tile::kBgA;
      walkables_.push_back(pos);

      // std::cout << "add walkable " << tileIndex << " : [" <<
      // ScreenPosition(tileIndex).x << " : " << ScreenPosition(tileIndex).y <<
      // "]\n";
    }
  }
}

void TileMap::Draw(sf::RenderWindow &window) {
  int tileIndex = 0;

  sf::Sprite sprite(textures.Get(Tile::kEmpty));

  sprite.setTextureRect(sf::IntRect({0, 0}, {kPixelStep, kPixelStep}));

  for (auto tile : tiles_) {
    sprite.setTexture(textures.Get(tile));
    sprite.setPosition(ScreenPosition(tileIndex));
    window.draw(sprite);

    tileIndex++;
  }
}

std::vector<sf::Vector2f> TileMap::GetWalkables() const { return walkables_; }

sf::Vector2f TileMap::ScreenPosition(const int index) {
  float x = ceil((index % (kWidth / kPixelStep)) * kPixelStep);
  float y = ceil((index / (kWidth / kPixelStep)) * kPixelStep);

  // std::cout << "Calculated Screen Position : " << x << "." << y << "\n";

  return {x, y};
}

int TileMap::Index(const sf::Vector2f screenPosition) {
  return static_cast<int>(ceil(screenPosition.y / kPixelStep * kWidth)) +
         static_cast<int>(ceil(screenPosition.x / kPixelStep));
}
