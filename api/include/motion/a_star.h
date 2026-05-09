#ifndef API_MOTION_A_STAR_H_
#define API_MOTION_A_STAR_H_

#include <SFML/System/Vector2.hpp>
#include <expected>

#include "ai/a_star.h"
#include "graphics/tilemap.h"
#include "motion/path.h"

namespace api::motion {
namespace Astar {

std::expected<Path, core::ai::pathfinding::PathError> GetPath(
    const TileMap& tilemap, sf::Vector2f start, sf::Vector2f end);

}
}  // namespace api::motion

#endif  // API_MOTION_A_STAR_H_
