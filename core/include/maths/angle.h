#ifndef CORE_MATHS_ANGLE_H_
#define CORE_MATHS_ANGLE_H_

#include "maths/const.h"

namespace core::maths {
class Degree;
class Radian {
 public:
  constexpr explicit Radian(float value) : value_(value) {}
  constexpr Radian(const Degree& angle);

  explicit constexpr operator float() const { return value_; }

 private:
  float value_;
};

class Degree {
 public:
  constexpr explicit Degree(float value) : value_(value) {}
  constexpr Degree(const Radian& angle)
      : value_(static_cast<float>(angle) / Pi * 180.0f) {}
  explicit constexpr operator float() const { return value_; }

 private:
  float value_;
};

constexpr Radian::Radian(const Degree& angle)
    : value_(static_cast<float>(angle) / 180.0f * Pi) {}
}  // namespace core::maths
#endif  // CORE_MATHS_ANGLE_H_
