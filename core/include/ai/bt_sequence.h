#ifndef CORE_AI_BT_SEQUENCE_H_
#define CORE_AI_BT_SEQUENCE_H_
#include "bt_composite.h"

namespace core::ai::behaviour_tree {
class Sequence : public Composite {
 public:
  Status Tick() override;
};
}  // namespace core::ai::behaviour_tree

#endif  // CORE_AI_BT_SEQUENCE_H_
