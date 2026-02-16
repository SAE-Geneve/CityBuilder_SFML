//
// Created by sebas on 11/06/2025.
//

#ifndef BT_SEQUENCE_H
#define BT_SEQUENCE_H
#include "bt_composite.h"

namespace core::ai::behaviour_tree {
class Sequence : public Composite {
 public:
  Status Tick() override;
};
}  // namespace core::ai::behaviour_tree

#endif  // BT_SEQUENCE_H
