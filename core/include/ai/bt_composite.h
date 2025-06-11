//
// Created by sebas on 11/06/2025.
//

#ifndef BT_COMPOSITE_H
#define BT_COMPOSITE_H

#include <memory>
#include <vector>

#include "bt_node.h"

namespace core::ai {
    namespace behaviour_tree {

        class Composite : public Node {

        protected:
            std::vector<std::unique_ptr<Node>> children_;
            int childIdx_ = 0;

        public:
            void Reset() override;
            void AddChild(std::unique_ptr<Node> child);

        };
    }
}

#endif //BT_COMPOSITE_H
