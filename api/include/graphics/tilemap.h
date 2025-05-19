#ifndef API_GRAPHICS_TILEMAP_H
#define API_GRAPHICS_TILEMAP_H

#include <SFML/Graphics.hpp>

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
    sf::Texture grass_texture_;
    sf::Texture default_texture_;

    static sf::Vector2f ScreenPosition(int index);
    static int Index(sf::Vector2f screenPosition);

public:
    void Setup();
    void Draw(sf::RenderWindow &window) const;

};
#endif
