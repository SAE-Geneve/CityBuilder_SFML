#include "ai/bt_selector.h"

#include "profiling/profiling.h"

namespace core::ai::behaviour_tree {

Status Selector::Tick() {
  PROFILE_ZONE();
  while (child_idx_ < std::size(children_)) {
    Status status = children_[child_idx_]->Tick();

    if (status == Status::kSuccess) {
      Reset();
      return Status::kSuccess;
    }

    if (status == Status::kRunning) {
      return Status::kRunning;
    }

    child_idx_++;
  }

  return Status::kFailure;
}

}  // namespace core::ai::behaviour_tree