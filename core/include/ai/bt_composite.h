//
// Created by sebas on 11/06/2025.
//

#ifndef BT_COMPOSITE_H
#define BT_COMPOSITE_H

#include <cstdlib>
#include <memory>
#include <span>
#include <vector>

#include "bt_node.h"

namespace core::ai::behaviour_tree {

class Composite : public Node {
 protected:
  std::vector<std::unique_ptr<Node>> children_;
  int64_t childIdx_ = 0;

 public:
  using Node::Node;
  Composite() = default;
  template <std::convertible_to<std::unique_ptr<Node>>... Ptrs>
  explicit Composite(Ptrs&&... ps) {
    children_.reserve(sizeof...(ps));
    (children_.emplace_back(std::forward<Ptrs>(ps)), ...);
  }
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
