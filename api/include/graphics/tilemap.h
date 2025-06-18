#ifndef API_GRAPHICS_TILEMAP_H
#define API_GRAPHICS_TILEMAP_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "noise/FastNoiseLite.h"
#include "assets/asset_manager.h"

constexpr int kWidth = 1600;
constexpr int kHeight = 1280;
constexpr int kPixelStep = 64;

using core::experimental::AssetManager;

namespace api::graphics
{

    enum class BgTile {
      kEmpty,
      kGrass16,
      kBgA,
      kBgB,
      kMaison,
      kWater,
      kLength
    };

    inline std::array<std::string_view, static_cast<size_t>(api::graphics::BgTile::kLength)> bg_files_ = {
      "empty.png", "grass.png", "bg_tile_a.png", "bg_tile_b.png", "maison.png",
      "water.png"
    };

  template<typename TTile>
  class TileMap {

    std::array<TTile, kWidth/kPixelStep * kHeight/kPixelStep> tiles_ = {};
    AssetManager<sf::Texture, TTile, "_assets/sprites"> textures;

    static sf::Vector2f ScreenPosition(int index);
    static int Index(sf::Vector2f screenPosition);

    std::vector<sf::Vector2f> walkables_;

public:
    explicit TileMap();
    void Setup(std::span<std::string_view> texture_names);
    void Draw(sf::RenderWindow &window);

    std::vector<sf::Vector2f> GetWalkables() const;
};

template<typename TTile>
TileMap<TTile>::TileMap() {}

template<typename TTile>
void TileMap<TTile>::Setup(std::span<std::string_view> texture_names) {
  // Create and configure FastNoise object
  FastNoiseLite noise;
  noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
  noise.SetSeed(1309);
  noise.SetFrequency(0.01f);

  textures.Load(texture_names);

  // Remplir le tableau avec de l'herbe ou de l'eau de manière aléatoire
  for (int tileIndex = 0; tileIndex < tiles_.size(); ++tileIndex) {
    sf::Vector2f pos = ScreenPosition(tileIndex);

    auto value = 0.5f * (noise.GetNoise(pos.x, pos.y) + 1);

    std::cout << "noise value = " << value << "\n";

    if (value < 0.35f) {
      tiles_[tileIndex] = TTile::kWater;
    } else {
      tiles_[tileIndex] = TTile::kBgA;
      walkables_.push_back(pos);
    }
  }
}

template<typename TTile>
void TileMap<TTile>::Draw(sf::RenderWindow &window) {
  int tileIndex = 0;

  sf::Sprite sprite(textures.Get(TTile::kEmpty));

  sprite.setTextureRect(sf::IntRect({0, 0}, {kPixelStep, kPixelStep}));

  for (auto tile : tiles_) {
    sprite.setTexture(textures.Get(tile));
    sprite.setPosition(ScreenPosition(tileIndex));
    window.draw(sprite);

    tileIndex++;
  }
}

template<typename TTile>
std::vector<sf::Vector2f> TileMap<TTile>::GetWalkables() const { return walkables_; }

template<typename TTile>
sf::Vector2f TileMap<TTile>::ScreenPosition(const int index) {
  float x = ceil((index % (kWidth / kPixelStep)) * kPixelStep);
  float y = ceil((index / (kWidth / kPixelStep)) * kPixelStep);

  // std::cout << "Calculated Screen Position : " << x << "." << y << "\n";

  return {x, y};
}

template<typename TTile>
int TileMap<TTile>::Index(const sf::Vector2f screenPosition) {
  return static_cast<int>(ceil(screenPosition.y / kPixelStep * kWidth)) +
         static_cast<int>(ceil(screenPosition.x / kPixelStep));
}
}
#endif
