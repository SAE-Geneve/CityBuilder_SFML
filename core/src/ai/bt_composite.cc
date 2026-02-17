//
// Created by sebas on 11/06/2025.
//

#include "ai/bt_composite.h"

#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

using namespace core::ai::behaviour_tree;

void Composite::Reset() {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  childIdx_ = 0;
}

void Composite::AddChild(std::unique_ptr<Node> child) {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  children_.push_back(std::move(child));
}