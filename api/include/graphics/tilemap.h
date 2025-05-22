#ifndef API_GRAPHICS_TILEMAP_H
#define API_GRAPHICS_TILEMAP_H

#include <SFML/Graphics.hpp>

#include "assets/asset_manager.h"

constexpr int kWidth = 1600;
constexpr int kHeight = 1280;
constexpr int kPixelStep = 16;

class TileMap {
    enum class Tile {
        EMPTY,
        GRASS
    };

private:
    std::array<Tile, kWidth/kPixelStep * kHeight/kPixelStep> tiles_ = {};
    /* TODO: delete textures direct usage
    sf::Texture grass_texture_;
    sf::Texture default_texture_;
    */
    AssetManager<sf::Texture> textures;

    static sf::Vector2f ScreenPosition(int index);
    static int Index(sf::Vector2f screenPosition);

public:
    explicit TileMap();
    void Setup();
    void Draw(sf::RenderWindow &window);

};
#endif
