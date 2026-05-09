#ifndef CORE_AI_BT_SELECTOR_H_
#define CORE_AI_BT_SELECTOR_H_

#include "bt_composite.h"

namespace core::ai::behaviour_tree {

class Selector : public Composite {
 public:
  Status Tick() override;
};

}  // namespace core::ai::behaviour_tree

#endif  // CORE_AI_BT_SELECTOR_H_
