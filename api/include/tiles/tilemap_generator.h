//
// Created by sebas on 20.05.2026.
//

#ifndef CITYBUILDER_TILEMAP_GENERATOR_H
#define CITYBUILDER_TILEMAP_GENERATOR_H

#include <random>
#include <ranges>
#include <span>
#include <__msvc_ranges_to.hpp>

#include "tile.h"

namespace tiles::generator {

    inline std::vector<Tile<TerrainTiles>> GenerateTerrain(sf::Vector2f size, sf::Vector2f offset){

        std::vector<Tile<TerrainTiles>> map;

        FastNoiseLite noise;
        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        noise.SetSeed(1337);
        noise.SetFrequency(0.0025f);

        for (float x = 0.f; x < size.x; x += offset.x) {// NOLINT(*-flp30-c)
            for (float y = 0.f; y < size.y; y += offset.y) {// NOLINT(*-flp30-c)

                // Generator stuff -----------------------------
                float noiseValue = abs(noise.GetNoise(x,y));
                if (noiseValue <= 0.3f) {
                    map.push_back(Tile{{x, y}, TerrainTiles::kGrassA});
                }else {
                    map.push_back(Tile{{x, y}, TerrainTiles::kWaterA});
                }

            }
        }
        return map;
    }

    inline std::vector<Tile<RessourcesTiles>> SeedAndGrow(std::span<Tile<TerrainTiles>> terrain, RessourcesTiles _seed){

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution rnd(0.f, 1.f);

        return terrain
        | std::views::filter([] (auto tile){ return tile.type == TerrainTiles::kGrassA;})
        | std::views::filter([&rnd, &gen] (auto tile){ return rnd(gen) <= 0.25f;})
        | std::views::transform([&_seed] (auto tile){ return Tile{tile.pos, _seed};})
        | std::ranges::to<std::vector<Tile<RessourcesTiles>>>();

    }

};


#endif //CITYBUILDER_TILEMAP_GENERATOR_H
