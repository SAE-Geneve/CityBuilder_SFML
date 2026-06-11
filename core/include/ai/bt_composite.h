//
// Created by sebas on 03.06.2026.
//

#ifndef CITYBUILDER_BT_COMPOSITE_H
#define CITYBUILDER_BT_COMPOSITE_H
#include <vector>

#include "bt_node.h"

namespace core::ai::behaviour_tree {
    class CompositeNode : public Node {
    protected:
        std::vector<std::unique_ptr<Node>> children_;
        int currentChild_ = 0;
        void Reset() override{ currentChild_ = 0; };
    public:
        CompositeNode() = default;
        ~CompositeNode() override = default;
        CompositeNode(const CompositeNode&) = delete;
        CompositeNode& operator=(const CompositeNode&) = delete;
        CompositeNode(CompositeNode&& node) noexcept : Node(std::move(node)){
            std::swap(children_, node.children_);
            currentChild_ = node.currentChild_;
        }
        CompositeNode& operator=(CompositeNode&& node) noexcept
        {
            Node::operator=(std::move(node));
            std::swap(children_, node.children_);
            currentChild_ = node.currentChild_;
            return *this;
        }

        void AddChild(std::unique_ptr<Node> child){
            children_.push_back(std::move(child));
        }

    };
}

#endif //CITYBUILDER_BT_COMPOSITE_H
