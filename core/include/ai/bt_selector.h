#ifndef CORE_AI_BT_SELECTOR_H
#define CORE_AI_BT_SELECTOR_H

#include "ai/bt_composite.h"

namespace core::ai::behaviour_tree {

// Selector = logical OR. Ticks children in order; succeeds as soon as one
// child succeeds, fails only when every child has failed.
class Selector : public Composite {
 public:
  Status Tick() override;
};

}  // namespace core::ai::behaviour_tree

#endif  // CORE_AI_BT_SELECTOR_H
