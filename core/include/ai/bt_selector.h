//
// Created by sebas on 05/06/2025.
//

#ifndef BT_SELECTOR_H
#define BT_SELECTOR_H

#include <memory>
#include <vector>

#include "bt_node.h"

namespace core::ai {
    namespace behaviour_tree {

        class Selector : public Node {

            std::vector<std::unique_ptr<Node>> children_;
            int childIdx_ = 0;

        public:
            void Reset() override;
            Status Tick() override;

            void AddChild(std::unique_ptr<Node> child);

        };

    }
}

#endif //BT_SELECTOR_H
