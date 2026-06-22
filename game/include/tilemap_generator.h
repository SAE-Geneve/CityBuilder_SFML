//
// Created by sebas on 20.05.2026.
//

#ifndef CITYBUILDER_TILEMAP_GENERATOR_H
#define CITYBUILDER_TILEMAP_GENERATOR_H

#include <print>
#include <random>
#include <ranges>
#include <span>

#include "../../api/include/tiles/tile.h"

namespace api::tiles::generator {
    inline std::random_device rd;
    inline std::mt19937 gen(rd());
    inline std::uniform_real_distribution rnd(0.f, 1.f);

    inline std::vector<Tile<TerrainTile> > GenerateTerrain(sf::Vector2i size, sf::Vector2f offset){
        std::vector<Tile<TerrainTile> > terrainMap;

        FastNoiseLite noise;
        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        noise.SetSeed(1337);
        noise.SetFrequency(0.0025f);

        for (float x = 0.f; x < size.x; x += offset.x) { // NOLINT(*-flp30-c)
            for (float y = 0.f; y < size.y; y += offset.y) { // NOLINT(*-flp30-c)

                // Generator stuff -----------------------------
                if (std::abs(noise.GetNoise(x, y)) <= 0.3f) {
                    if (rnd(gen) < 0.5f) {
                        terrainMap.emplace_back(Tile{{{x, y}, true}, TerrainTile::kGrassA});
                    } else {
                        terrainMap.emplace_back(Tile{{{x, y}, true}, TerrainTile::kGrassB});
                    }
                } else {
                    if (rnd(gen) < 0.5f) {
                        terrainMap.emplace_back(Tile{{{x, y}, true}, TerrainTile::kWaterA});
                    } else {
                        terrainMap.emplace_back(Tile{{{x, y}, true}, TerrainTile::kWaterB});
                    }
                }
            }
        }
        return terrainMap;
    }

    inline void SeedAndGrow(std::vector<Tile<ResourceTile> > &base_resources, ResourceTile seed, float limit){
        for (auto &tile: base_resources) {
            if (tile.type == ResourceTile::kUndefined) {
                float rng_value = rnd(gen);
                // std::println("Rng Tile : {}", rng_value);
                if (rng_value < limit) {
                    tile.type = seed;
                }
            }
        }
    }

    inline std::vector<Tile<ResourceTile> > FilterTerrain(std::span<Tile<TerrainTile> > terrainMap){
        auto map = terrainMap
                   | std::views::filter([](auto tile) {
                       return tile.type == TerrainTile::kGrassA || tile.type == TerrainTile::kGrassB;
                   })
                   | std::views::transform([](auto tile) {
                       return Tile<ResourceTile>{{tile.Pos, true}, ResourceTile::kUndefined};
                   })
                   | std::ranges::to<std::vector<Tile<ResourceTile> > >();

        return map;
    }
};


#endif //CITYBUILDER_TILEMAP_GENERATOR_H
