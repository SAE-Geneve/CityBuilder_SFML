//
// Created by sebas on 07.05.2026.
//

#include "graphics/tilemap.h"

namespace graphics {

    void TileMap::Setup(sf::Texture *texture, sf::Vector2i gridSize, sf::Vector2f gridOffset, sf::Vector2f tileSize) {
        // init textures
        texture_ = texture;
        vertices_.clear();

        constexpr sf::Color color = sf::Color::White;

        for (int x = 0; x < gridSize.x; x += static_cast<int>(gridOffset.x)) {
            for (int y = 0; y < gridSize.x; y += static_cast<int>(gridOffset.x)) {


                vertices_.append(sf::Vertex({static_cast<float>(x), static_cast<float>(y)}, color, {0, 0}));
                vertices_.append(sf::Vertex({static_cast<float>(x) + gridOffset.x, static_cast<float>(y)}, color,
                                            {tileSize.x, 0}));
                vertices_.append(sf::Vertex(
                    {static_cast<float>(x) + gridOffset.x, static_cast<float>(y) + gridOffset.y}, color, tileSize));

                vertices_.append(sf::Vertex({static_cast<float>(x), static_cast<float>(y)}, color, {0, 0}));
                vertices_.append(sf::Vertex(
                    {static_cast<float>(x) + gridOffset.x, static_cast<float>(y) + gridOffset.y}, color, tileSize));
                vertices_.append(sf::Vertex({static_cast<float>(x), static_cast<float>(y) + gridOffset.y}, color,
                                            {0, tileSize.y}));
            }
        }
    }

    void TileMap::Draw(sf::RenderWindow &window) const {
        sf::RenderStates states;
        states.texture = texture_;

        window.draw(vertices_, states);
    }
} // namespace graphics
