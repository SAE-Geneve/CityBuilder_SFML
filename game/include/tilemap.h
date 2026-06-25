//
// Created by sebas on 20.05.2026.
//

#ifndef CITYBUILDER_TILEMAP_H
#define CITYBUILDER_TILEMAP_H

#include <random>
#include <mdspan>
#include <SFML/Graphics/Export.hpp>

#include "FastNoiseLite.h"
#include "game_types.h"
#include "graphics/camera.h"
#include "graphics/tilemap_renderer.h"
#include "graphics/tilesheet.h"
#include "ai/a_star_graph.h"

namespace game {

class Tilemap {

    sf::Vector2i grid_size_;
    sf::Vector2i grid_offset_;

    api::graphics::TilemapRenderer terrain_renderer_;
    api::graphics::Tilesheet<TerrainTile> terrain_tilesheet_;

    api::graphics::TilemapRenderer resources_renderer_;
    api::graphics::Tilesheet<ResourceTile> resources_tilesheet_;

    std::vector<TerrainTile> terrain_;
    std::vector<ResourceTile> resources_;

public:
    void Setup(sf::Vector2i grid_size, sf::Vector2i gridOffset, api::ai::AStarGraph&aStarGraph);
    void Draw(sf::RenderWindow &window);

};


}

#endif //CITYBUILDER_TILEMAP_H
