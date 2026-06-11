//
// Created by sebas on 03.06.2026.
//

#ifndef CITYBUILDER_BT_SELECTOR_H
#define CITYBUILDER_BT_SELECTOR_H
#include <vector>

#include "bt_composite.h"

namespace core::ai::behaviour_tree {
    class SelectorNode : public CompositeNode {
    public:
        SelectorNode() = default;
        ~SelectorNode() override = default;
        SelectorNode(const SelectorNode&) = delete;
        SelectorNode& operator=(const SelectorNode&) = delete;
        SelectorNode(SelectorNode&& node) noexcept : CompositeNode(std::move(node)){}
        SelectorNode& operator=(SelectorNode&& node) noexcept
        {
            CompositeNode::operator=(std::move(node));
            return *this;
        }

        Status Tick() override{
            Status status = children_[currentChild_]->Tick();
            if (status == Status::kFailure) {
                currentChild_++;
                if (currentChild_ >= children_.size()) {
                    Reset();
                    return Status::kFailure;
                }
                return Tick();
            }

            return status;
        }
    };



}

#endif //CITYBUILDER_BT_SELECTOR_H
