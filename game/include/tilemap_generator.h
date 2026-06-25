//
// Created by sebas on 20.05.2026.
//

#ifndef CITYBUILDER_TILEMAP_GENERATOR_H
#define CITYBUILDER_TILEMAP_GENERATOR_H

#include <random>
#include <ranges>
#include <span>

#include "game_types.h"
#include "rng/rng.h"
#include "tiles/world_settings.h"

namespace game::generator {

    inline std::vector<TerrainTile> GenerateTerrain(sf::Vector2i size, sf::Vector2i offset){
        std::vector terrainMap(size.x * size.y, TerrainTile::kUndefined);

        FastNoiseLite noise;
        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        noise.SetSeed(1337);
        noise.SetFrequency(0.0025f);

        for (int x = 0.f; x < size.x; x ++) { // NOLINT(*-flp30-c)
            for (int y = 0.f; y < size.y; y ++) { // NOLINT(*-flp30-c)

                // Generator stuff -----------------------------
                if (std::abs(noise.GetNoise(static_cast<float>(x * offset.x), static_cast<float>(y * offset.y))) <= 0.3f) {
                    if (core::rng::get_value(0.f,1.f) < 0.5f) {
                        terrainMap.at(api::tiles::WorldSettings::TilePosToIdx(sf::Vector2i{x,y})) = TerrainTile::kGrassA;
                    } else {
                        terrainMap.at(api::tiles::WorldSettings::TilePosToIdx(sf::Vector2i{x,y})) = TerrainTile::kGrassB;
                    }
                } else {
                    if (core::rng::get_value(0.f,1.f) < 0.5f) {
                        terrainMap.at(api::tiles::WorldSettings::TilePosToIdx(sf::Vector2i{x,y})) = TerrainTile::kWaterA;
                    } else {
                        terrainMap.at(api::tiles::WorldSettings::TilePosToIdx(sf::Vector2i{x,y})) = TerrainTile::kWaterB;
                    }
                }
            }
        }
        return terrainMap;
    }

    inline void SeedAndGrow(std::vector<ResourceTile> &base_resources, const std::vector<TerrainTile> &base_terrain, ResourceTile seed, float limit){

        for (int idxTile = 0; idxTile < base_resources.size(); ++idxTile) {

            auto& tile = base_resources[idxTile];

            if (base_terrain[idxTile] == TerrainTile::kGrassA || base_terrain[idxTile] == TerrainTile::kGrassB) {
                if (tile == ResourceTile::kFree) {
                    // std::println("Rng Tile : {}", rng_value);
                    if (core::rng::get_value(0.f, 1.f) < limit) {
                        tile = seed;
                    }
                }
            }
        }

        //
        // for (auto &tile: base_resources) {
        //     if (tile == ResourceTile::kUndefined) {
        //         // std::println("Rng Tile : {}", rng_value);
        //         if (core::rng::get_value(0.f, 1.f) < limit) {
        //             tile = seed;
        //         }
        //     }
        // }
    }

    inline std::vector<ResourceTile> FilterTerrain(std::span<TerrainTile> terrainMap){
        auto map = terrainMap
                   | std::views::transform([](auto tile) {
                       return ResourceTile::kFree;
                   })
                   | std::ranges::to<std::vector<ResourceTile>>();

        return map;
    }
};


#endif //CITYBUILDER_TILEMAP_GENERATOR_H
