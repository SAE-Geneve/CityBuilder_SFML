#include "motion/path.h"

#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

using namespace api::motion;

bool Path::IsDone() const {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  return idxPoint_ == path_points_.size() - 1;
}

bool Path::IsValid() const {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  return !path_points_.empty();
}

sf::Vector2f Path::GetNextPoint() {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  if (!IsValid()) {
    return {0, 0};
  }

  if (!IsDone()) {
    ++idxPoint_;
  }

  return path_points_[idxPoint_];
}

void Path::Fill(const std::vector<sf::Vector2f> &path_points) {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  if (!path_points.empty()) {
    path_points_ = path_points;
    idxPoint_ = 0;
  }
}

sf::Vector2f Path::StartPoint() const {
#ifdef TRACY_ENABLE
  ZoneScoped;
#endif
  if (IsValid()) {
    return path_points_[0];
  }
  return {
      0,
      0,
  };
}
