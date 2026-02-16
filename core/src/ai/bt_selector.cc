//
// Created by sebas on 05/06/2025.
//

#include "ai/bt_selector.h"

using namespace core::ai::behaviour_tree;

Status Selector::Tick() {
  while (childIdx_ < children_.size()) {
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