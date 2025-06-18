#ifndef NPC_H
#define NPC_H

#include <SFML/Graphics.hpp>

#include "ai/bt_node.h"
#include "assets/asset_manager.h"
#include "graphics/tilemap.h"
#include "motion/motor.h"
#include "motion/path.h"

using namespace core::ai::behaviour_tree;
using namespace api::motion;

namespace api::ai {
    class Npc {

    public:
      // Assets
        enum class Sprites {
            kEmpty,
            kIdle,
            kLength
        };
    private:
        //std::string_view files_[static_cast<size_t>(Sprites::kLength)] = {"empty.png", "scifiUnit_06.png"};
        core::experimental::AssetManager<sf::Texture, Sprites, "_assets/sprites"> textures;

        // Behaviour tree
        std::unique_ptr<Node> root_;
        // Behaviour Constants
        static constexpr float kHungerRate = 0.1f;
        static constexpr float kMovingSpeed = 50.0f;
      // Behaviours fields
      float hunger_ = 0;
      bool resourceAvailable_ = true;
      bool target_reachable_ = true;
      float target_distance_ = 20;

        // Motion
        Motor motor_;
        Path path_;

        // Tilemap
        const TileMap* tileMap_ = nullptr;

    public:
        void Setup(const TileMap* tileMap, std::span<std::string_view> files);
        void Update(float dt);
        void Draw(sf::RenderWindow &window);

        void SetupBehaviourTree();
      // Actions
      Status Move();
      Status Eat();

        // Motion
        void SetPath(const Path& path);

    };
}

#endif //NPC_H
