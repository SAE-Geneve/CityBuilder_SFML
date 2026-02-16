#ifndef ASTAR_H
#define ASTAR_H

#include <SFML/System/Vector2.hpp>
#include <filesystem>

#include "motion/path.h"

namespace api::motion {
namespace Astar {

Path GetPath(int gridStep, sf::Vector2f start, sf::Vector2f end,
             std::vector<sf::Vector2f> walkableTiles);

}
}  // namespace api::motion

#endif  // ASTAR_H
