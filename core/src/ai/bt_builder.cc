#include "ai/bt_builder.h"

#include "ai/bt_selector.h"
#include "ai/bt_sequence.h"

namespace core::ai::behaviour_tree {

TreeBuilder& TreeBuilder::Sequence() {
  // "class Sequence" disambiguates the type from the member function Sequence().
  auto node = std::make_unique<class Sequence>();
  auto* raw = node.get();
  Attach(std::move(node));
  stack_.push(raw);
  return *this;
}

TreeBuilder& TreeBuilder::Selector() {
  auto node = std::make_unique<class Selector>();
  auto* raw = node.get();
  Attach(std::move(node));
  stack_.push(raw);
  return *this;
}

TreeBuilder& TreeBuilder::End() {
  stack_.pop();
  return *this;
}

std::unique_ptr<Node> TreeBuilder::Build() { return std::move(root_); }

void TreeBuilder::Attach(std::unique_ptr<Node> node) {
  if (stack_.empty()) {
    root_ = std::move(node);
  } else {
    stack_.top()->AddChild(std::move(node));
  }
}

}  // namespace core::ai::behaviour_tree
