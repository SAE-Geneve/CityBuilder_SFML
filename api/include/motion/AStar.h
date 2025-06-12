#ifndef ASTAR_H
#define ASTAR_H
#include <filesystem>
#include <__msvc_filebuf.hpp>
#include <SFML/System/Vector2.hpp>

#include "motion/path.h"

namespace motion {
    namespace Astar {
        api::motion::Path GetPath(sf::Vector2f start, sf::Vector2f end, std::vector<sf::Vector2f> walkableTiles);
    }
}


#endif //ASTAR_H
