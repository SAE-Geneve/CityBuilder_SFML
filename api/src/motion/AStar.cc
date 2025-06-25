#include "motion/AStar.h"

#include <array>
#include <iostream>
#include <queue>

#include "motion/path.h"

namespace api::motion {
    namespace Astar {

        struct aStarNode {
            sf::Vector2f position;
            float g;
            float h;
            float f;

            aStarNode *previous_node;

            aStarNode(sf::Vector2f position, float g, float h, aStarNode *prev) : position(position),
                g(g),
                h(h),
                f(g + h),
                previous_node(prev){
            }

            // A Star node are prioritize by the lowest f value
            friend bool operator<(aStarNode const &left, aStarNode const &right){
                return left.f > right.f;
            }
        };

        float heuristic(sf::Vector2f p1, sf::Vector2f p2){
            return (p2 - p1).length();
        }

        std::array<sf::Vector2f, 4> neighbours(int gridStep = 16){
            std::array<sf::Vector2f, 4> neighbours = {
                sf::Vector2f(0, gridStep),
                sf::Vector2f(gridStep, 0),
                sf::Vector2f(0, -1 * gridStep),
                sf::Vector2f(-1 * gridStep, 0)
            };

            return neighbours;
        }

        Path ReconstitutePath(aStarNode &start_node){
            Path path;
            std::vector<sf::Vector2f> pathPoints;
            aStarNode *current_node = &start_node;

            while (current_node != nullptr) {
                // std::cout << "reconstiution point : " << current_node->position.x << ":" << current_node->position.y <<
                //         std::endl;
                pathPoints.emplace_back(current_node->position);
                current_node = current_node->previous_node;
            }

            std::reverse(pathPoints.begin(), pathPoints.end());
            path.Fill(pathPoints);

            return path;
        }

        Path GetPath(const int gridStep, sf::Vector2f start, sf::Vector2f end,
                     std::vector<sf::Vector2f> walkableTiles){
            Path aStarPath;

            // Are start / end point in walkables tiles ?
            auto f = std::find(walkableTiles.begin(), walkableTiles.end(), start);
            if (f == walkableTiles.end()) {
                std::cout << "Start point (" << start.x << ":" << start.y << ")not in walkable tiles" << std::endl;
                return aStarPath;
            }

            auto g = std::find(walkableTiles.begin(), walkableTiles.end(), end);
            if (g == walkableTiles.end()) {
                std::cout << "End point (" << end.x << ":" << end.y << ") not in walkable tiles" << std::endl;
                return aStarPath;
            }

            // ---------------------------------------------
            std::vector<aStarNode> closedList;

            std::priority_queue<aStarNode> openList;
            openList.push(aStarNode(start, 0, heuristic(start, end), nullptr));

            while (!openList.empty()) {
                aStarNode currentNode = openList.top();
                openList.pop();

                //std::cout << "current node : " << currentNode.position.x << ":" << currentNode.position.y << std::endl;

                if (currentNode.position == end) {
                    std::cout << "Found path" << std::endl;
                    return ReconstitutePath(currentNode);
                }

                for (auto neighbour: neighbours(gridStep)) {
                    sf::Vector2f newPosition = currentNode.position + neighbour;

                    auto f = std::find(walkableTiles.begin(), walkableTiles.end(), newPosition);

                    if (f != walkableTiles.end()) {
                        auto g = std::find_if(
                            closedList.begin(),
                            closedList.end(),
                            [&newPosition](const aStarNode &n) {
                                return newPosition == n.position;
                            }
                        );

                        // new node
                        aStarNode newNode = aStarNode(
                            newPosition,
                            currentNode.g + 1,
                            heuristic(newPosition, end),
                            new aStarNode(currentNode)
                        );

                        if (g == closedList.end()) {
                            openList.push(newNode);
                        }
                        closedList.emplace_back(newNode);
                    }
                }
            }

            return aStarPath;
        }
    }
}
