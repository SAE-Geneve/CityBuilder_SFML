//
// Created by unite on 28.05.2025.
//

#ifndef CORE_MATHS_ANGLE_H
#define CORE_MATHS_ANGLE_H

#include "maths/const.h"

namespace core {
class Degree;
class Radian {
 public:
  constexpr explicit Radian(float value): value_(value){}
  constexpr Radian(const Degree& angle);

  explicit constexpr operator float() const { return value_; }

 private:
  float value_;
};

class Degree {
public:
  constexpr explicit Degree(float value): value_(value){}
  constexpr Degree(const Radian& angle): value_(static_cast<float>(angle)/Pi*180.0f) {}
  explicit constexpr operator float() const {
    return value_;
  }

private:
  float value_;
};


constexpr Radian::Radian(const Degree& angle)
    : value_(static_cast<float>(angle) / 180.0f * Pi) {}
}
#endif //CORE_MATHS_ANGLE_H
