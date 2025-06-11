//
// Created by sebas on 11/06/2025.
//

#ifndef BT_SEQUENCE_H
#define BT_SEQUENCE_H
#include "bt_composite.h"

namespace core::ai {
    namespace behaviour_tree {
        class Sequence : public Composite {

        public:
            Status Tick() override;

        };
    }
}

#endif //BT_SEQUENCE_H
