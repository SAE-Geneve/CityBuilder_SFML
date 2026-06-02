#include "ai/bt_selector.h"

namespace core::ai::behaviour_tree {

Status Selector::Tick() {
  while (childIdx_ < static_cast<int>(children_.size())) {
    const Status status = children_[childIdx_]->Tick();

    if (status == Status::kSuccess) {
      Reset();
      return Status::kSuccess;
    }

    if (status == Status::kRunning) {
      return Status::kRunning;
    }

    childIdx_++;
  }

  return Status::kFailure;
}

}  // namespace core::ai::behaviour_tree
