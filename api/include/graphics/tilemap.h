//
// Created by sebas on 07.05.2026.
//

#ifndef CITYBUILDER_TILEMAP_H
#define CITYBUILDER_TILEMAP_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace graphics {
    class TileMap {

    private:
        sf::Vector2f gridSize_;
        sf::Vector2f gridOffset_;
        sf::Texture* texture_ = nullptr;
        sf::VertexArray vertices_ = sf::VertexArray(sf::PrimitiveType::Triangles);

    public:
        void Setup(sf::Texture *texture, sf::Vector2i gridSize, sf::Vector2f gridOffset, sf::Vector2f tileSize);
        void Draw(sf::RenderWindow &window) const;

    };
}

#endif  // CITYBUILDER_TILEMAP_H
