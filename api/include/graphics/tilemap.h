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
        kEmpty, kBg, kWater,
        kRock, kTree, kFood,
        kLength
    };
    std::array<std::string_view, static_cast<size_t>(TileMap::Tile::kLength)> files_ = {
        "empty.png", "bg_tile_a.png", "water.png",
        "bg_plus_rock.png", "bg_plus_tree.png", "bg_plus_food.png"
    };

    std::array<Tile, kWidth/kPixelStep * kHeight/kPixelStep> tiles_ = {};
    AssetManager<sf::Texture, Tile, "_assets/sprites"> textures_;

    static sf::Vector2f ScreenPosition(int index);
    static int Index(sf::Vector2f screenPosition);

    std::vector<sf::Vector2f> walkables_;

public:
    void Setup();
    void Draw(sf::RenderWindow &window);

    std::vector<sf::Vector2f>& GetWalkables();
};
#endif
