#include "motion/path.h"

#include "profiling/profiling.h"

namespace api::motion {

bool Path::IsDone() const {
  return path_points_.empty() || idx_point_ == path_points_.size() - 1;
}

bool Path::IsValid() const {
  return !path_points_.empty();
}

sf::Vector2f Path::NextPoint() {
  PROFILE_ZONE();
  if (!IsValid()) {
    return {0, 0};
  }

  if (!IsDone()) {
    ++idx_point_;
  }

  return path_points_[idx_point_];
}

void Path::Fill(const std::vector<sf::Vector2f>& path_points) {
  PROFILE_ZONE();
  if (!path_points.empty()) {
    path_points_ = path_points;
    idx_point_ = 0;
  }
}

sf::Vector2f Path::StartPoint() const {
  if (IsValid()) {
    return path_points_[0];
  }
  return {0, 0};
}

}  // namespace api::motion
