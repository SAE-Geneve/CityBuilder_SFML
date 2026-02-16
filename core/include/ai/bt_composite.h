//
// Created by sebas on 11/06/2025.
//

#ifndef BT_COMPOSITE_H
#define BT_COMPOSITE_H

#include <memory>
#include <vector>

#include "bt_node.h"

namespace core::ai::behaviour_tree {

class Composite : public Node {
 protected:
  std::vector<std::unique_ptr<Node>> children_;
  int childIdx_ = 0;

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
};
}  // namespace core::ai::behaviour_tree

#endif  // BT_COMPOSITE_H
