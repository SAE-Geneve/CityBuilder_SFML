//
// Created by sebas on 11/06/2025.
//

#include "ai/bt_sequence.h"

#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

using namespace core::ai::behaviour_tree;

Status Sequence::Tick() {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  while (childIdx_ < children_.size()) {
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
