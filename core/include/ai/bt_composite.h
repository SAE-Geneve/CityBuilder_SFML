#ifndef CORE_AI_BT_COMPOSITE_H_
#define CORE_AI_BT_COMPOSITE_H_

#include <cstdlib>
#include <memory>
#include <span>
#include <vector>

#include "bt_node.h"

namespace core::ai::behaviour_tree {

class Composite : public Node {
 protected:
  std::vector<std::unique_ptr<Node>> children_;
  size_t child_idx_ = 0;

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
        child_idx_(other.child_idx_) {}
  Composite& operator=(Composite&& other) noexcept {
    Node::operator=(std::move(other));
    std::swap(children_, other.children_);
    child_idx_ = other.child_idx_;
    return *this;
  }

  void Reset() override;
  void AddChild(std::unique_ptr<Node> child);
};
}  // namespace core::ai::behaviour_tree

#endif  // CORE_AI_BT_COMPOSITE_H_
