//
// Created by unite on 28.05.2025.
//

#ifndef CORE_MATHS_FUNC_H
#define CORE_MATHS_FUNC_H
#include <cmath>

namespace core {

float Cos(Radian angle) {
  return std::cos(static_cast<float>(angle));
}
float Sin(Radian angle) {
  return std::sin(static_cast<float>(angle));
}
float Tan(Radian angle) {
  return std::tan(static_cast<float>(angle));
}

}

#endif //CORE_MATHS_FUNC_H
