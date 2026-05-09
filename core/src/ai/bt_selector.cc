#include "ai/bt_selector.h"

#include "profiling/profiling.h"

namespace core::ai::behaviour_tree {

Status Selector::Tick() {
  PROFILE_ZONE();
  while (childIdx_ < std::size(children_)) {
    Status status = children_[childIdx_]->Tick();

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