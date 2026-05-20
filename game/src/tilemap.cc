//
// Created by sebas on 20.05.2026.
//

#include "tilemap.h"

#include "game_types.h"

void Tilemap::Setup(sf::Vector2f gridSize, sf::Vector2f gridOffset){

    if (tilesheet_.InitTileSheet("_assets/tiles/RTS_medieval@2_no_margins.png", 128)) {
        tilesheet_.AddTile(BackgroundTiles::kGrassA, 0, 0);
        tilesheet_.AddTile(BackgroundTiles::kGrassB, 1, 0);
        tilesheet_.AddTile(BackgroundTiles::kWaterA, 0, 2);
        tilesheet_.AddTile(BackgroundTiles::kWaterB, 1, 2);


        // init textures -------------------------------------------------------------------
        renderer_.SetTexture(tilesheet_.GetTexture());
        renderer_.Clear();

        FastNoiseLite noise;
        noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        noise.SetSeed(1337);
        noise.SetFrequency(0.0025f);

        for (float x = 0.f; x < gridSize.x; x += gridOffset.x) { // NOLINT(*-flp30-c)
            for (float y = 0.f; y < gridSize.y; y += gridOffset.x) { // NOLINT(*-flp30-c)

                // Generator stuff -----------------------------
                sf::FloatRect texBounds;

                float noiseValue = noise.GetNoise(x,y);
                if (noiseValue > 0.0f) {
                    texBounds = tilesheet_.GetBounds(BackgroundTiles::kGrassA);
                }else {
                    texBounds = tilesheet_.GetBounds(BackgroundTiles::kWaterA);
                }

                renderer_.AddTile({x, y}, gridOffset, texBounds);
            }
        }

    }
}

void Tilemap::Draw(sf::RenderWindow &window){
    renderer_.Draw(window);
}
