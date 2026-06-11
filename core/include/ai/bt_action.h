//
// Created by sebas on 03.06.2026.
//

#ifndef CITYBUILDER_BT_ACTION_H
#define CITYBUILDER_BT_ACTION_H

#include <functional>
#include "bt_node.h"

namespace core::ai::behaviour_tree{

    template<std::invocable Taction>
    class ActionNode : public Node{

        Taction callback_action_;

    public:
        ActionNode(const Taction& callback_action) : callback_action_(callback_action) {}

        ~ActionNode() override = default;
        ActionNode(const ActionNode&) = delete;
        ActionNode& operator=(const ActionNode&) = delete;
        ActionNode(ActionNode&& node) noexcept : Node(std::move(node)){}
        ActionNode& operator=(Node&& node) noexcept {
            Node::operator=(std::move(node));
            return *this;
        }

        void Reset() override {}
        Status Tick() override{
            return callback_action_();
        }
    };


}

#endif //CITYBUILDER_BT_ACTION_H
