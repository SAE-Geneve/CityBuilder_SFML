#ifndef API_GRAPHICS_TILEMAP_H
#define API_GRAPHICS_TILEMAP_H

#include <SFML/Graphics.hpp>

#include "assets/asset_manager.h"

constexpr int kWidth = 1600;
constexpr int kHeight = 1280;
constexpr int kPixelStep = 64;

using core::experimental::AssetManager;

class TileMap {
    enum class Tile {
        kEmpty,
        kGrass16,
        kBgA,
        kBgB,
        kMaison,
        kWater,
        kLength
    };

    std::string_view files[static_cast<size_t>(Tile::kLength)] {
        "empty.png", "grass.png", "bg_tile_a.png", "bg_tile_b.png", "maison.png",
        "water.png"
    };

private:
    std::array<Tile, kWidth/kPixelStep * kHeight/kPixelStep> tiles_ = {};
    AssetManager<sf::Texture, Tile, "_assets/sprites"> textures;

    static sf::Vector2f ScreenPosition(int index);
    static int Index(sf::Vector2f screenPosition);

    std::vector<sf::Vector2f> walkables_;

public:
    explicit TileMap();
    void Setup();
    void Draw(sf::RenderWindow &window);

    std::vector<sf::Vector2f> GetWalkables() const;
};
#endif
