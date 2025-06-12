//
// Created by sebas on 11/06/2025.
//

#include "motion/path.h"

#include <iostream>

using namespace api::motion;

bool Path::IsDone() const{
    return idxPoint_ == pathPoints_.size() - 1;
}

bool Path::IsValid() const{
    return !pathPoints_.empty();
}

sf::Vector2f Path::GetNextPoint(){

    if (!IsValid()) {
        return {0, 0};
    }

    if (!IsDone()) {
        ++idxPoint_;
    }

    return pathPoints_[idxPoint_];
}

void Path::Fill(std::vector<sf::Vector2f>& pathPoints){
    if (!pathPoints.empty())
        pathPoints_ = pathPoints;
}

sf::Vector2f Path::StartPoint() const{
    if (IsValid()) {
        return pathPoints_[0];
    }
    return {0, 0,};
}

