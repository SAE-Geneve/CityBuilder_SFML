#ifndef BT_NODE_H
#define BT_NODE_H

#include <algorithm>


namespace core::ai::behaviour_tree {

enum class Status { kFailure, kRunning, kSuccess };

class Node {
 public:
  //Implementation of the rule of five, movable-only
  Node() = default;
  virtual ~Node() = default;
  Node(const Node&) = delete;
  Node& operator=(const Node&) = delete;
  Node(Node&& node) noexcept {
    std::swap(status_, node.status_);
  }
  Node& operator=(Node&& node) noexcept {
    std::swap(status_, node.status_);
    return *this;
  }
  virtual void Reset() = 0;
  virtual Status Tick() = 0;

 protected:
  Status status_ = Status::kFailure;
};

} // namespace core::ai::behaviour_tree


#endif  // BT_NODE_H
