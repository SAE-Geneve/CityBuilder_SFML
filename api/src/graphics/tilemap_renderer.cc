//
// Created by sebas on 07.05.2026.
//

#include "graphics/tilemap_renderer.h"

namespace graphics {
    void TilemapRenderer::AddTile(sf::Vector2f pos, sf::Vector2f offset, sf::FloatRect texBounds){
        vertices_.append(sf::Vertex(pos, color, texBounds.position));
        vertices_.append(sf::Vertex(pos + sf::Vector2f(offset.x, 0.f), color,
                                    texBounds.position + sf::Vector2f(texBounds.size.x, 0.f))); // + X
        vertices_.append(sf::Vertex(pos + offset, color, texBounds.position + texBounds.size));

        vertices_.append(sf::Vertex(pos, color, texBounds.position));
        vertices_.append(sf::Vertex(pos + offset, color, texBounds.position + texBounds.size));;
        vertices_.append(sf::Vertex(pos + sf::Vector2f(0.f, offset.y), color,
                                    texBounds.position + sf::Vector2f(0.f, texBounds.size.y))); // + Y
    }

    void TilemapRenderer::Setup(sf::Texture *texture, sf::Vector2f gridSize,
                                sf::Vector2f gridOffset, sf::FloatRect texBounds){
        // init textures
        texture_ = texture;
        vertices_.clear();

        for (float x = 0.f; x < gridSize.x; x += gridOffset.x) { // NOLINT(*-flp30-c)
            for (float y = 0.f; y < gridSize.y;
                 y += gridOffset.x) { // NOLINT(*-flp30-c)

                AddTile({x, y}, gridOffset, texBounds);
            }
        }
    }

    void TilemapRenderer::Draw(sf::RenderWindow &window) const{
        sf::RenderStates states;
        if (texture_) {
            states.texture = texture_;
        }
        if (vertices_.getVertexCount() > 0) {
            window.draw(vertices_, states);
        }
    }
} // namespace graphics
