//
// Created by sebas on 05/06/2025.
//

#include "ai/bt_selector.h"

using namespace core::ai::behaviour_tree;

void Selector::AddChild(std::unique_ptr<Node> child){
    children_.push_back(std::move(child));
}

void Selector::Reset(){
    childIdx_ = 0;
}

Status Selector::Tick(){

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