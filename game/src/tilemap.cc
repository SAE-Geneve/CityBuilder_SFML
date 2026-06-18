//
// Created by sebas on 20.05.2026.
//

#include "tilemap.h"

#include "game_types.h"
#include "../include/tilemap_generator.h"

void Tilemap::Setup(sf::Vector2i grid_size, sf::Vector2f gridOffset, api::ai::AStarGraph &astar_graph){
    using namespace api::tiles;

    grid_size_ = grid_size;
    grid_offset_ = gridOffset;

    terrain_ = generator::GenerateTerrain(grid_size_, gridOffset);

    resources_ = generator::FilterTerrain(terrain_);

    generator::SeedAndGrow(resources_, ResourceTile::kWood, 0.05f);
    generator::SeedAndGrow(resources_, ResourceTile::kRock, 0.05f);
    generator::SeedAndGrow(resources_, ResourceTile::kFood, 0.20f);

    resources_ = resources_
    | std::views::filter([](auto tile){return tile.type != ResourceTile::kUndefined;})
    | std::ranges::to<std::vector>();

    if (terrain_tilesheet_.InitTileSheet("_assets/tiles/RTS_medieval@2_no_margins_transparent.png", 128)) {
        terrain_tilesheet_.AddTile(TerrainTile::kGrassA, 0, 0);
        terrain_tilesheet_.AddTile(TerrainTile::kGrassB, 1, 0);
        terrain_tilesheet_.AddTile(TerrainTile::kWaterA, 0, 2);
        terrain_tilesheet_.AddTile(TerrainTile::kWaterB, 1, 2);


        // init textures -------------------------------------------------------------------
        terrain_renderer_.SetTexture(terrain_tilesheet_.GetTexture());
        terrain_renderer_.ClearVertices();

        for (auto &tile: terrain_) {
            if (tile.type == TerrainTile::kGrassA || tile.type == TerrainTile::kGrassB) {
                astar_graph.AddNode(sf::Vector2i{tile.Pos});
            }
            terrain_renderer_.AddTile(tile.Pos, gridOffset, terrain_tilesheet_.GetBounds(tile.type));
        }
    }

    if (resources_tilesheet_.InitTileSheet("_assets/tiles/RTS_medieval@2_no_margins_transparent.png", 128)) {
        resources_tilesheet_.AddTile(ResourceTile::kWood, 5, 3);
        resources_tilesheet_.AddTile(ResourceTile::kRock, 5, 4);
        resources_tilesheet_.AddTile(ResourceTile::kFood, 5, 5);


        // init textures -------------------------------------------------------------------
        resources_renderer_.SetTexture(resources_tilesheet_.GetTexture());
        resources_renderer_.ClearVertices();

        for (auto &tile: resources_) {
            astar_graph.RemoveNode(sf::Vector2i{tile.Pos});
            resources_renderer_.AddTile(tile.Pos, gridOffset, resources_tilesheet_.GetBounds(tile.type));
        }
    }

}

void Tilemap::Draw(sf::RenderWindow &window){
    terrain_renderer_.Draw(window);
    resources_renderer_.Draw(window);
}
