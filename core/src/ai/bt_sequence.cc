#include "ai/bt_sequence.h"

#include "profiling/profiling.h"

namespace core::ai::behaviour_tree {

Status Sequence::Tick() {
  PROFILE_ZONE();
  while (child_idx_ < std::size(children_)) {
    Status status = children_[child_idx_]->Tick();

    if (status == Status::kFailure) {
      Reset();
      return Status::kFailure;
    }

    if (status == Status::kRunning) {
      return Status::kRunning;
    }

    child_idx_++;
  }

  Reset();
  return Status::kSuccess;
}

}  // namespace core::ai::behaviour_tree
