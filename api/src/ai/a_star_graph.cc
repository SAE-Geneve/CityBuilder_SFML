//
// Created by sebas on 11.06.2026.
//


#include "ai/a_star_graph.h"

#include <vector>

namespace api::ai {
    void AStarGraph::AddNode(sf::Vector2i node){
        graph_.emplace(node);
    }

    void AStarGraph::RemoveNode(const sf::Vector2i node){
        graph_.erase(node);
    }

    bool AStarGraph::ContainsNode(const sf::Vector2i node) const{
        return graph_.contains(node);
    }

    std::vector<sf::Vector2i> AStarGraph::GetPath(sf::Vector2i start, sf::Vector2i end){
        std::vector<sf::Vector2i> path;

        path.push_back(start);
        path.push_back(end);

        return path;
    }
}