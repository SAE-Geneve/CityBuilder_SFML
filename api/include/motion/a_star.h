#ifndef ASTAR_H
#define ASTAR_H

#include <filesystem>
#include <SFML/System/Vector2.hpp>

#include "motion/path.h"

namespace api::motion {
    namespace Astar {

        Path GetPath(int gridStep, sf::Vector2f start, sf::Vector2f end, std::vector<sf::Vector2f> walkableTiles);

    }
}


#endif //ASTAR_H
