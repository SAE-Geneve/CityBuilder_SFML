#include "motion/path.h"

using namespace api::motion;

bool Path::IsDone() const{
    return idxPoint_ == path_points_.size() - 1;
}

bool Path::IsValid() const{
    return !path_points_.empty();
}

sf::Vector2f Path::GetNextPoint(){

    if (!IsValid()) {
        return {0, 0};
    }

    if (!IsDone()) {
        ++idxPoint_;
    }

    return path_points_[idxPoint_];
}

void Path::Fill(const std::vector<sf::Vector2f> &path_points){
    if (!path_points.empty()) {
        path_points_ = path_points;
        idxPoint_ = 0;
    }
}

sf::Vector2f Path::StartPoint() const{
    if (IsValid()) {
        return path_points_[0];
    }
    return {0, 0,};
}

