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

class Tilemap {

    sf::Vector2f grid_size_;
    sf::Vector2f grid_offset_;

    api::graphics::TilemapRenderer terrain_renderer_;
    api::graphics::Tilesheet<TerrainTile> terrain_tilesheet_;

    api::graphics::TilemapRenderer resources_renderer_;
    api::graphics::Tilesheet<ResourceTile> resources_tilesheet_;

    std::vector<api::tiles::Tile<TerrainTile> > terrain_;
    std::vector<api::tiles::Tile<ResourceTile> > resources_;

public:
    void Setup(sf::Vector2f grid_size, sf::Vector2f gridOffset, api::ai::AStarGraph&aStarGraph);
    void Draw(sf::RenderWindow &window);

};

#include "ai/a_star_graph.h"

#include "graphics/tilemap_renderer.h"

#include "graphics/tilesheet.h"

#endif //CITYBUILDER_TILEMAP_H
