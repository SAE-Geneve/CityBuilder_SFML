//
// Created by sebas on 03.06.2026.
//

#ifndef CITYBUILDER_BT_SEQUENCE_H
#define CITYBUILDER_BT_SEQUENCE_H
#include <vector>

#include "bt_composite.h"
#include "bt_node.h"

namespace core::ai::behaviour_tree{
    class SequenceNode : public CompositeNode {
        public:
        SequenceNode() = default;
        ~SequenceNode() override = default;
        SequenceNode(const SequenceNode&) = delete;
        SequenceNode& operator=(const SequenceNode&) = delete;
        SequenceNode(SequenceNode&& node) noexcept : CompositeNode(std::move(node)){}
        SequenceNode& operator=(SequenceNode&& node) noexcept
        {
            CompositeNode::operator=(std::move(node));
            return *this;
        }

        Status Tick() override{

            Status status = children_[currentChild_]->Tick();
            if (status == Status::kSuccess) {
                currentChild_++;
                if (currentChild_ >= children_.size()) {
                    Reset();
                    return Status::kSuccess;
                }
                return Tick();
            }

            return status;

        }
    };

}

#endif //CITYBUILDER_BT_SEQUENCE_H
