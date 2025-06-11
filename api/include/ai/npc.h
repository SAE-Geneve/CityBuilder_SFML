#ifndef NPC_H
#define NPC_H

#include <SFML/Graphics.hpp>

#include "ai/bt_action.h"
#include "ai/bt_selector.h"
#include "assets/asset_manager.h"

using namespace core::ai::behaviour_tree;

class Npc {

    enum class Animation {
        kEmpty,
        KBlue,
        kLength
    };
    std::string_view files[static_cast<size_t>(Animation::kLength)]{"empty.png", "npc_blue.png"};

    core::experimental::AssetManager<sf::Texture, Animation, "_assets/sprites"> textures;

    std::unique_ptr<Node> root_;

    // Constants
    static constexpr float kHungerRate = 0.1f;
    static constexpr float kMovingSpeed = 0.5f;

public:
    void Setup();
    void Update();
    void Draw(sf::RenderWindow &window);

    // Actions
    Status Move();
    Status Eat();

    // Behaviours
    //bool hungry_ = false;
    float hunger_ = 0;
    bool resourceAvailable_ = true;
    bool target_reachable_ = true;
    float target_distance_ = 20;

};

#endif //NPC_H
