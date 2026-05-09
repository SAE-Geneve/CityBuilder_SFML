#ifndef CORE_AI_BT_ACTION_H_
#define CORE_AI_BT_ACTION_H_

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

  void Reset() override {}

  Status Tick() override {
    // Faire des trucs ---------------
    return action_();
  }
};

namespace experimental {
class Action : public Node {
protected:
  virtual Status RunAction() = 0;
public:
  void Reset() override {}

  Status Tick() override {
    return RunAction();
  }
};
}

} // namespace core::ai::behaviour_tree


#endif  // CORE_AI_BT_ACTION_H_
