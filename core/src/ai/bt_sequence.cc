//
// Created by sebas on 11/06/2025.
//

#include "ai/bt_sequence.h"

#include "profiling/profiling.h"

using namespace core::ai::behaviour_tree;

Status Sequence::Tick() {
  PROFILE_ZONE();
  while (childIdx_ < std::ssize(children_)) {
    Status status = children_[childIdx_]->Tick();

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
