#include "motion/a_star.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <queue>

#include "motion/path.h"

#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

namespace api::motion::Astar {

struct aStarNode {
  sf::Vector2f position;
  float g;
  float h;
  float f;

  aStarNode *previous_node;

  aStarNode(sf::Vector2f position, float g, float h, aStarNode *prev)
      : position(position), g(g), h(h), f(g + h), previous_node(prev) {}

  // A Star node are prioritize by the lowest f value
  friend bool operator<(aStarNode const &left, aStarNode const &right) {
    return left.f > right.f;
  }
};

float heuristic(sf::Vector2f p1, sf::Vector2f p2) {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  return (p2 - p1).length();
}

std::array<sf::Vector2f, 4> neighbours(int gridStep = 16) {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  std::array<sf::Vector2f, 4> neighbours = {
      sf::Vector2f(0, gridStep), sf::Vector2f(gridStep, 0),
      sf::Vector2f(0, -1 * gridStep), sf::Vector2f(-1 * gridStep, 0)};

  return neighbours;
}

Path ReconstitutePath(aStarNode &start_node) {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  Path path;
  std::vector<sf::Vector2f> pathPoints;
  aStarNode *current_node = &start_node;

  while (current_node != nullptr) {
    // std::cout << "reconstiution point : " << current_node->position.x << ":"
    // << current_node->position.y <<
    //         std::endl;
    pathPoints.emplace_back(current_node->position);
    current_node = current_node->previous_node;
  }

  std::ranges::reverse(pathPoints);
  path.Fill(pathPoints);

  return path;
}

Path GetPath(const int gridStep, const sf::Vector2f start,
             const sf::Vector2f end,
             const std::vector<sf::Vector2f> walkableTiles) {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  Path aStarPath;

  // Are start / end point in walkables tiles ?
  auto f = std::ranges::find(walkableTiles, start);
  if (f == walkableTiles.end()) {
    std::cout << "Start point (" << start.x << ":" << start.y
              << ")not in walkable tiles" << std::endl;
    return aStarPath;
  }

  auto g = std::ranges::find(walkableTiles, end);
  if (g == walkableTiles.end()) {
    std::cout << "End point (" << end.x << ":" << end.y
              << ") not in walkable tiles" << std::endl;
    return aStarPath;
  }

  // ---------------------------------------------
  std::vector<aStarNode> closedList;

  std::priority_queue<aStarNode> openList;
  openList.push(aStarNode(start, 0, heuristic(start, end), nullptr));

  while (!openList.empty()) {
    aStarNode currentNode = openList.top();
    openList.pop();

    // std::cout << "current node : " << currentNode.position.x << ":" <<
    // currentNode.position.y << std::endl;

    if (currentNode.position == end) {
      std::cout << "Found path" << std::endl;
      return ReconstitutePath(currentNode);
    }

    for (auto neighbour : neighbours(gridStep)) {
      sf::Vector2f newPosition = currentNode.position + neighbour;

      auto f =
          std::ranges::find(walkableTiles, newPosition);

      if (f != walkableTiles.end()) {
        auto g = std::ranges::find_if(closedList,
                              [&newPosition](const aStarNode &n) {
                                return newPosition == n.position;
                              });

        //FIXME when is this "new aStarNode(currentNode)" getting deleted? This is no C#, this is memory leak!
        // new node
        aStarNode newNode =
            aStarNode(newPosition, currentNode.g + 1,
                      heuristic(newPosition, end), new aStarNode(currentNode));

        if (g == closedList.end()) {
          openList.push(newNode);
        }
        closedList.emplace_back(newNode);
      }
    }
  }

  return aStarPath;
}
}  // namespace api::motion::Astar
