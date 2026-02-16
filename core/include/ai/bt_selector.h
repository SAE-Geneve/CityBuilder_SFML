//
// Created by sebas on 05/06/2025.
//

#ifndef BT_SELECTOR_H
#define BT_SELECTOR_H

#include "bt_composite.h"

namespace core::ai::behaviour_tree {

class Selector : public Composite {
 public:
  Status Tick() override;
};

}  // namespace core::ai::behaviour_tree

#endif  // BT_SELECTOR_H
