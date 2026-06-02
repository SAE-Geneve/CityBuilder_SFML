#include "ai/bt_sequence.h"

namespace core::ai::behaviour_tree {

Status Sequence::Tick() {
  while (childIdx_ < static_cast<int>(children_.size())) {
    const Status status = children_[childIdx_]->Tick();

    if (status == Status::kFailure) {
      Reset();
      return Status::kFailure;
    }

    if (status == Status::kRunning) {
      return Status::kRunning;
    }

    childIdx_++;
  }

  Reset();
  return Status::kSuccess;
}

}  // namespace core::ai::behaviour_tree
