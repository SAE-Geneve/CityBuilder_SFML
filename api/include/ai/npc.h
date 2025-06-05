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

public:
    void Setup();
    void Update();
    void Draw(sf::RenderWindow &window);

    // Actions
    Status Move();

    // Behaviours
    //bool hungry_ = false;
    int hunger_ = 0;
    bool resourceAvailable_ = true;

};

#endif //NPC_H
