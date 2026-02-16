#ifndef ACTION_H
#define ACTION_H

#include <functional>

#include "bt_node.h"

namespace core::ai::behaviour_tree {

//TODO use inheritance instead of std::function
class Action : public Node {
 private:
  std::function<Status()> action_;

 public:
  explicit Action(std::function<Status()> action)
      : action_(std::move(action)) {}

  void Reset() override {};

  Status Tick() override {
    // Faire des trucs ---------------
    return action_();
  }
};

} // namespace core::ai::behaviour_tree


#endif  // ACTION_H
