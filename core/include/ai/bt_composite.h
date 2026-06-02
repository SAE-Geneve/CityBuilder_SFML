#ifndef CORE_AI_BT_COMPOSITE_H
#define CORE_AI_BT_COMPOSITE_H

#include <memory>
#include <utility>
#include <vector>

#include "ai/bt_node.h"

namespace core::ai::behaviour_tree {

// Common base for Sequence / Selector: owns its children and remembers
// which child it is currently ticking (childIdx_) so a kRunning child can
// be resumed on the next tick.
class Composite : public Node {
 public:
  using Node::Node;

  Composite(Composite&& other) noexcept
      : Node(std::move(other)),
        children_(std::move(other.children_)),
        childIdx_(other.childIdx_) {}
  Composite& operator=(Composite&& other) noexcept {
    Node::operator=(std::move(other));
    std::swap(children_, other.children_);
    childIdx_ = other.childIdx_;
    return *this;
  }

  void Reset() override;
  void AddChild(std::unique_ptr<Node> child);

 protected:
  std::vector<std::unique_ptr<Node>> children_;
  int childIdx_ = 0;
};

}  // namespace core::ai::behaviour_tree

#endif  // CORE_AI_BT_COMPOSITE_H
