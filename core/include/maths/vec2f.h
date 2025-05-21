#ifndef VEC2F_H
#define VEC2F_H

namespace core {
class Vec2f {
public:
  float x = 0.0f, y = 0.0f;

  constexpr Vec2f operator+(const Vec2f & v2) const noexcept {
    return {x+v2.x, y+v2.y};
  }

};
}
#endif //VEC2F_H
