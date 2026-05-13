//
// Created by sebas on 07.05.2026.
//

#ifndef CITYBUILDER_TILESHEET_H
#define CITYBUILDER_TILESHEET_H

#include <SFML/Graphics/Rect.hpp>

namespace graphics::tilesheet {
    inline sf::FloatRect ConstructRect(float xIdx, float yIdx, float width, float height) {
        return {{width * xIdx, yIdx * height}, {width, height}};
    }
}

#endif //CITYBUILDER_TILESHEET_H
