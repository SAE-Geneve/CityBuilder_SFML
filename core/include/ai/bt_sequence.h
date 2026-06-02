#ifndef CORE_AI_BT_SEQUENCE_H
#define CORE_AI_BT_SEQUENCE_H

#include "ai/bt_composite.h"

namespace core::ai::behaviour_tree {

// Sequence = logical AND. Ticks children in order; fails as soon as one
// child fails, succeeds only when every child has succeeded.
class Sequence : public Composite {
 public:
  Status Tick() override;
};

}  // namespace core::ai::behaviour_tree

#endif  // CORE_AI_BT_SEQUENCE_H
