//
// Created by sebas on 20.05.2026.
//

#include "tilemap.h"

#include "game_types.h"
#include "tilemap_generator.h"

namespace game {
    void Tilemap::Setup(sf::Vector2i grid_size, sf::Vector2i gridOffset, api::ai::AStarGraph &astar_graph){
        using namespace api::tiles;

        grid_size_ = grid_size;
        grid_offset_ = gridOffset;

        terrain_ = generator::GenerateTerrain(grid_size_, gridOffset);
        resources_ = generator::FilterTerrain(terrain_);

        generator::SeedAndGrow(resources_, terrain_, ResourceTile::kWood, 0.05f);
        generator::SeedAndGrow(resources_, terrain_, ResourceTile::kRock, 0.05f);
        generator::SeedAndGrow(resources_, terrain_, ResourceTile::kFood, 0.20f);

        // resources_ = resources_
        //              | std::views::filter([](auto tile) { return tile != ResourceTile::kUndefined; })
        //              | std::ranges::to<std::vector>();

        if (terrain_tilesheet_.InitTileSheet("_assets/tiles/RTS_medieval@2_no_margins_transparent.png", 128)) {
            terrain_tilesheet_.AddTile(TerrainTile::kGrassA, 0, 0);
            terrain_tilesheet_.AddTile(TerrainTile::kGrassB, 1, 0);
            terrain_tilesheet_.AddTile(TerrainTile::kWaterA, 0, 2);
            terrain_tilesheet_.AddTile(TerrainTile::kWaterB, 1, 2);


            // init textures -------------------------------------------------------------------
            terrain_renderer_.SetTexture(terrain_tilesheet_.GetTexture());
            terrain_renderer_.ClearVertices();


            for (size_t idxTerrain = 0; idxTerrain < terrain_.size(); ++idxTerrain) {
                const TerrainTile &tile = terrain_[idxTerrain];
                const sf::Vector2i pos{WorldSettings::IdxToTilePos(idxTerrain)};

                if (tile == TerrainTile::kGrassA || tile == TerrainTile::kGrassB) {
                    astar_graph.SetWalkablePos(idxTerrain);
                }else {
                    astar_graph.UnsetWalkablePos(idxTerrain);
                }
                terrain_renderer_.AddTile(
                    {static_cast<float>(pos.x) * grid_offset_.x, static_cast<float>(pos.y) * grid_offset_.y},
                    sf::Vector2f{gridOffset}, terrain_tilesheet_.GetBounds(tile));
            }
        }

        if (resources_tilesheet_.InitTileSheet("_assets/tiles/RTS_medieval@2_no_margins_transparent.png", 128)) {
            resources_tilesheet_.AddTile(ResourceTile::kWood, 5, 3);
            resources_tilesheet_.AddTile(ResourceTile::kRock, 5, 4);
            resources_tilesheet_.AddTile(ResourceTile::kFood, 5, 5);


            // init textures -------------------------------------------------------------------
            resources_renderer_.SetTexture(resources_tilesheet_.GetTexture());
            resources_renderer_.ClearVertices();

            for (int idxResource = 0; idxResource < resources_.size(); ++idxResource) {
                const ResourceTile &resource_tile = resources_[idxResource];
                const TerrainTile &terrain_tile = terrain_[idxResource];
                const sf::Vector2i pos{WorldSettings::IdxToTilePos(idxResource)};

                if (resource_tile == ResourceTile::kFree && (terrain_tile == TerrainTile::kGrassA || terrain_tile == TerrainTile::kGrassB)) {
                    astar_graph.SetWalkablePos({pos.x, pos.y});
                }else {
                    astar_graph.UnsetWalkablePos({pos.x, pos.y});
                }

                resources_renderer_.AddTile(
                    {static_cast<float>(pos.x) * grid_offset_.x, static_cast<float>(pos.y) * grid_offset_.y},
                    sf::Vector2f{gridOffset}, resources_tilesheet_.GetBounds(resource_tile));
            }

        }

    }

    void Tilemap::Draw(sf::RenderWindow &window){
        terrain_renderer_.Draw(window);
        resources_renderer_.Draw(window);
    }
}
