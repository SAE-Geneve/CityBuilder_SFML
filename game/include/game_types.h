//
// Created by sebas on 13.05.2026.
//

#ifndef CITYBUILDER_GAME_TYPES_H
#define CITYBUILDER_GAME_TYPES_H

#include "tiles/tile.h"

enum class TerrainTile : uint8_t {
    kUndefined,
    kGrassA,
    kGrassB,
    kWaterA,
    kWaterB
};

enum class ResourceTile : uint8_t {
    kFree,
    kWood,
    kRock,
    kFood
};

#endif //CITYBUILDER_GAME_TYPES_H
