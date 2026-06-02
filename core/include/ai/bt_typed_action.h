#ifndef CORE_AI_BT_TYPED_ACTION_H
#define CORE_AI_BT_TYPED_ACTION_H

#include <memory>
#include <utility>

#include "ai/bt_node.h"

namespace core::ai::behaviour_tree {

// Leaf node holding a concrete callable *by value*. Unlike Action (which uses
// std::function), there is no type erasure: no hidden heap allocation, and the
// call can be inlined. The callable type is captured in the template parameter.
template <typename Fn>
class TypedAction : public Node {
 public:
  explicit TypedAction(Fn fn) : fn_(std::move(fn)) {}

  void Reset() override {}
  Status Tick() override { return fn_(); }

 private:
  Fn fn_;
};

// Factory: deduces Fn so callers never spell the type out.
template <typename Fn>
std::unique_ptr<Node> MakeAction(Fn fn) {
  return std::make_unique<TypedAction<Fn>>(std::move(fn));
}

}  // namespace core::ai::behaviour_tree

#endif  // CORE_AI_BT_TYPED_ACTION_H
