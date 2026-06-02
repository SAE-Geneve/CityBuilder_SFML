#ifndef CORE_AI_BT_ACTION_H
#define CORE_AI_BT_ACTION_H

#include <functional>
#include <utility>

#include "ai/bt_node.h"

namespace core::ai::behaviour_tree {

// A leaf node: runs a callable returning a Status.
// TODO (TP3): replace std::function by a typed action to avoid type erasure.
class Action : public Node {
 public:
  explicit Action(std::function<Status()> action)
      : action_(std::move(action)) {}

  void Reset() override {}

  Status Tick() override { return action_(); }

 private:
  std::function<Status()> action_;
};

}  // namespace core::ai::behaviour_tree

#endif  // CORE_AI_BT_ACTION_H
