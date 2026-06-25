//
// Created by sebas on 11.06.2026.
//


#include "ai/a_star_graph.h"

#include <algorithm>
#include <queue>
#include <ranges>
#include <unordered_map>
#include <vector>

#include "rng/rng.h"
#include "tiles/world_settings.h"

namespace api::ai {

    void AStarGraph::SetWalkablePos(const size_t idx){
        if (idx < walkables_.size()) {
            walkables_.at(idx) = WalkableState::Walkable;
        }
    }
    void AStarGraph::UnsetWalkablePos(const size_t idx){
        if (idx < walkables_.size()) {
            walkables_.at(idx) = WalkableState::NotWalkable;
        }
    }

    void AStarGraph::SetWalkablePos(sf::Vector2i node){
        walkables_.at(api::tiles::WorldSettings::TilePosToIdx(node)) = WalkableState::Walkable;
    }

    void AStarGraph::UnsetWalkablePos(const sf::Vector2i node){
        walkables_.at(api::tiles::WorldSettings::TilePosToIdx(node)) = WalkableState::NotWalkable;
    }

    bool AStarGraph::IsInBoundsAndWalkable(const sf::Vector2i pixel_pos) const{
        if (std::optional<size_t> idx = tiles::WorldSettings::PixelPosToIdx(pixel_pos); idx.has_value()) {
            return walkables_.at(idx.value()) == WalkableState::Walkable;
        }
        return false;
    }

    sf::Vector2i AStarGraph::GetRandomNode() const{

        size_t rnd_idx;
        do {
            rnd_idx = core::rng::get_value(static_cast<size_t>(0), walkables_.size() - 1);
        }while (walkables_.at(rnd_idx) != WalkableState::Walkable);

        return tiles::WorldSettings::IdxToPixelPos(rnd_idx);

        // auto grid_pos = api::tiles::WorldSettings::IdxToTilePos(rnd_idx);
        // return {grid_pos.x * api::tiles::WorldSettings::tile_size.x, grid_pos.y * api::tiles::WorldSettings::tile_size.y};
    }

    std::vector<sf::Vector2i> AStarGraph::GetPath(sf::Vector2i start, sf::Vector2i end) const{
        std::priority_queue<AStarVertex, std::vector<AStarVertex>, VxCompareByF> open_queue;

        // came_from_ (a reused member) doubles as the closed set: a position is a key iff
        // it has been settled, and the stored value is the parent we reached it from. This
        // gives O(1) membership AND the reverse chain for path reconstruction.
        came_from_.clear();

        sf::Vector2i rounded_start = {start.x - (start.x % api::tiles::WorldSettings::tile_size.x),  start.y - (start.y % api::tiles::WorldSettings::tile_size.y)};
        sf::Vector2i rounded_end = {end.x - (end.x % api::tiles::WorldSettings::tile_size.x),  end.y - (end.y % api::tiles::WorldSettings::tile_size.y)};

        if (!IsInBoundsAndWalkable(rounded_start) || !IsInBoundsAndWalkable(rounded_end)) {
            return {};
        }

        // Start is its own parent; the reconstruction loop stops at rounded_start so it is never followed.
        open_queue.push(AStarVertex(rounded_start, 0, ManhattanDistance(rounded_start, rounded_end), rounded_start));

        while (!open_queue.empty()) {

            AStarVertex v = open_queue.top(); open_queue.pop();

            // Closed-set membership in O(1): skip if this position was already settled.
            if (came_from_.contains(v.position)) {
                continue;
            }
            came_from_.emplace(v.position, v.parent_position);

            if (v.position == rounded_end){
                // Walk the reverse chain from end back to start (start excluded), then reverse.
                std::vector<sf::Vector2i> path;
                for (sf::Vector2i cur = rounded_end; cur != rounded_start; cur = came_from_.at(cur)) {
                    path.push_back(cur);
                }
                std::ranges::reverse(path);
                return path;
            }

            for (sf::Vector2i neighbour: kNeighbours) {
                sf::Vector2i new_position = v.position + sf::Vector2i{neighbour.x * tiles::WorldSettings::tile_size.x, neighbour.y * tiles::WorldSettings::tile_size.y};
                if (!came_from_.contains(new_position) && IsInBoundsAndWalkable(new_position)) {
                    open_queue.push(AStarVertex(new_position, v.g + tiles::WorldSettings::tile_size.x, ManhattanDistance(new_position, rounded_end), v.position));
                }
            }
        }

        return {};
    }

    int ManhattanDistance(sf::Vector2i a, sf::Vector2i b){
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }
}
