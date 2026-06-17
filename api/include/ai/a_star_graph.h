//
// Created by sebas on 11.06.2026.
//

#ifndef CITYBUILDER_PATH_H
#define CITYBUILDER_PATH_H

#include <unordered_set>
#include <vector>
#include <SFML/System/Vector2.hpp>

namespace api::ai {

    // TODO : Slide about hash and unordered_set
    struct Vec2iHash {
        size_t operator()(const sf::Vector2i& node) const noexcept{
            size_t h = std::hash<int>{}(node.x);
            h ^= std::hash<int>{}(node.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
    //
    // // TODO : Slide about hash and unordered_set
    // struct Vec2iEq {
    //     bool operator()(const sf::Vector2i& a,const sf::Vector2i& b) const noexcept{return a.x == b.x && a.y == b.y;}
    // };

    class AStarGraph {

        // TODO : Slide about hash
        std::unordered_set<sf::Vector2i, Vec2iHash> graph_;

    public:
        explicit AStarGraph() = default;

        void AddNode(sf::Vector2i node);
        void RemoveNode(sf::Vector2i node);
        [[nodiscard]] bool ContainsNode(sf::Vector2i node) const;

        std::vector<sf::Vector2i> GetPath(sf::Vector2i start, sf::Vector2i end);

    };
}
#endif //CITYBUILDER_PATH_H
