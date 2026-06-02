#ifndef CORE_AI_BT_BUILDER_H
#define CORE_AI_BT_BUILDER_H

#include <memory>
#include <stack>
#include <utility>

#include "ai/bt_composite.h"
#include "ai/bt_node.h"
#include "ai/bt_typed_action.h"

namespace core::ai::behaviour_tree {

// Fluent builder: the shape of the calls mirrors the shape of the tree.
//   TreeBuilder()
//     .Selector()
//       .Sequence().Action(a).Action(b).End()
//       .Action(c)
//     .End()
//     .Build();
// Composites are pushed on a stack; Action() attaches a leaf to the current
// composite; End() closes the current composite.
class TreeBuilder {
 public:
  TreeBuilder& Sequence();
  TreeBuilder& Selector();
  TreeBuilder& End();
  std::unique_ptr<Node> Build();

  // Leaf node: stores the callable by value via TypedAction (no std::function).
  template <typename Fn>
  TreeBuilder& Action(Fn fn) {
    Attach(MakeAction(std::move(fn)));
    return *this;
  }

 private:
  void Attach(std::unique_ptr<Node> node);

  std::unique_ptr<Node> root_;
  std::stack<Composite*> stack_;
};

}  // namespace core::ai::behaviour_tree

#endif  // CORE_AI_BT_BUILDER_H
