//
// Created by sebas on 11/06/2025.
//

#include "ai/bt_composite.h"

using namespace core::ai::behaviour_tree;

void Composite::Reset(){
    childIdx_ = 0;
}

void Composite::AddChild(std::unique_ptr<Node> child){
    children_.push_back(std::move(child));
}