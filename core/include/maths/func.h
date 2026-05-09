#ifndef CORE_MATHS_FUNC_H_
#define CORE_MATHS_FUNC_H_
#include <cmath>

#include "maths/angle.h"

namespace core::maths {

inline float Cos(Radian angle) { return std::cos(static_cast<float>(angle)); }
inline float Sin(Radian angle) { return std::sin(static_cast<float>(angle)); }
inline float Tan(Radian angle) { return std::tan(static_cast<float>(angle)); }

}  // namespace core::maths

#endif  // CORE_MATHS_FUNC_H_
