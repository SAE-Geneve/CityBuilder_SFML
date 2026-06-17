//
// Created by sebas on 20.05.2026.
//

#ifndef CITYBUILDER_TILEMAP_GENERATOR_H
#define CITYBUILDER_TILEMAP_GENERATOR_H

#include <random>
#include <ranges>
#include <span>

#include "tile.h"

namespace api::tiles::generator {

    inline std::vector<Tile<TerrainTile>> GenerateTerrain(sf::Vector2i size, sf::Vector2f offset){

        std::vector<Tile<TerrainTile>> terrainMap;

        FastNoiseLite noise;
        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        noise.SetSeed(1337);
        noise.SetFrequency(0.0025f);

        for (float x = 0.f; x < size.x; x += offset.x) {// NOLINT(*-flp30-c)
            for (float y = 0.f; y < size.y; y += offset.y) {// NOLINT(*-flp30-c)

                // Generator stuff -----------------------------
                if (std::abs(noise.GetNoise(x,y)) <= 0.3f) {
                    terrainMap.emplace_back(Tile{{{x, y} , true}, TerrainTile::kGrassA});
                }else {
                    terrainMap.emplace_back(Tile{{{x,y}, false}, TerrainTile::kWaterA});
                }

            }
        }
        return terrainMap;
    }

    inline std::vector<Tile<ResourceTile>> SeedAndGrow(std::span<Tile<TerrainTile>> terrain, ResourceTile _seed){

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution rnd(0.f, 1.f);

        std::vector<Tile<ResourceTile>> ressourceMap;

        auto map = terrain
        | std::views::filter([] (auto tile){ return tile.type == TerrainTile::kGrassA;})
        | std::views::filter([&rnd, &gen] (auto tile){ return rnd(gen) <= 0.25f;})
        | std::views::transform([&_seed] (auto tile){ return Tile<ResourceTile>{{tile.Pos, false}, _seed};});

        for (auto tile: map) {
            ressourceMap.emplace_back(tile);
        }
        return ressourceMap;

    }

};


#endif //CITYBUILDER_TILEMAP_GENERATOR_H
