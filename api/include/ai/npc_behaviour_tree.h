//
// Created by sebas on 19/06/2025.
//

#ifndef NPC_BEHAVIOUR_TREE_H
#define NPC_BEHAVIOUR_TREE_H

#include <memory>

#include "ai/bt_node.h"
#include "graphics/tilemap.h"
#include "motion/motor.h"
#include "motion/path.h"

namespace api::ai {
    class NpcBehaviourTree {
        // Behaviour tree
        std::unique_ptr<core::ai::behaviour_tree::Node> bt_root_;
        motion::Motor *npc_motor_ = nullptr;
        TileMap *tilemap_ = nullptr;
        motion::Path *path_ = nullptr;


        void SetRandomDestination() const;
        // Actions
        [[nodiscard]] core::ai::behaviour_tree::Status CheckHunger() const;
        [[nodiscard]] core::ai::behaviour_tree::Status Move() const;
        [[nodiscard]] core::ai::behaviour_tree::Status Eat();
        [[nodiscard]] core::ai::behaviour_tree::Status Work();
        [[nodiscard]] core::ai::behaviour_tree::Status Idle();

        // Behaviour Constants
        static constexpr float kHungerRate = 0.1f;

        // Behaviours
        float hunger_ = 0.0f;
        bool resourceAvailable_ = true;
        // bool target_reachable_ = true;
        // float target_distance_ = 20;

    public:
        //NpcBehaviourTree() = default;
        //NpcBehaviourTree(const NpcBehaviourTree&) = delete;
        //NpcBehaviourTree& operator=(const NpcBehaviourTree&) = delete;

        void SetupBehaviourTree(
            motion::Motor* npc_motor,
            motion::Path* path,
            TileMap* tilemap);
        void Update(float dt);

    };
}

#endif //NPC_BEHAVIOUR_TREE_H
