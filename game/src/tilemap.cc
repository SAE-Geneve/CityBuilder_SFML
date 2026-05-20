//
// Created by sebas on 20.05.2026.
//

#include "tilemap.h"

#include "game_types.h"
#include "../../api/include/tiles/tilemap_generator.h"

void Tilemap::Setup(sf::Vector2f gridSize, sf::Vector2f gridOffset){

    std::vector<tiles::Tile<TerrainTiles>> terrain = tiles::generator::GenerateTerrain({1920, 1080}, {32, 32});


    if (tilesheet_.InitTileSheet("_assets/tiles/RTS_medieval@2_no_margins.png", 128)) {
        tilesheet_.AddTile(TerrainTiles::kGrassA, 0, 0);
        tilesheet_.AddTile(TerrainTiles::kGrassB, 1, 0);
        tilesheet_.AddTile(TerrainTiles::kWaterA, 0, 2);
        tilesheet_.AddTile(TerrainTiles::kWaterB, 1, 2);


        // init textures -------------------------------------------------------------------
        renderer_.SetTexture(tilesheet_.GetTexture());
        renderer_.Clear();

        for (auto &tile: terrain) {
            renderer_.AddTile(tile.pos, gridOffset, tilesheet_.GetBounds(tile.type));
        }
    }
}

void Tilemap::Draw(sf::RenderWindow &window){
    renderer_.Draw(window);
}
