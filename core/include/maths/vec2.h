
#ifndef MATHS_VEC2_H
#define MATHS_VEC2_H
#include <cmath>
#include <type_traits>

namespace core {

template <typename VectorT, typename T>
concept is_vector2 = requires(VectorT v) {
  { v.x } -> std::same_as<T&>;
  { v.y } -> std::same_as<T&>;
};

template <typename VectorT, typename T>
concept is_convertible_to_vector2 = requires(VectorT v) {
  { v.x } -> std::convertible_to<T>;
  { v.y } -> std::convertible_to<T>;
};

template <typename T>
  requires std::is_arithmetic_v<T>
class Vec2 {
 public:
  T x{}, y{};
  constexpr Vec2() = default;
  constexpr Vec2(T x, T y) : x{x}, y{y} {}

  template <typename VectorT>
    requires is_vector2<VectorT, T>
  constexpr explicit Vec2(VectorT v) : x(v.x), y(v.y) {}

  template <typename VectorT>
    requires is_convertible_to_vector2<VectorT, T> && !is_vector2<VectorT, T>
  constexpr explicit Vec2(VectorT v)
      : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

  [[nodiscard]] Vec2 operator+(Vec2 other) const {
    return {x + other.x, y + other.y};
  }

  template <typename VectorT>
  requires is_vector2<VectorT, T>
  explicit constexpr operator VectorT() const {
    return VectorT{x, y};
  }

  static float Dot(Vec2 v1, Vec2 v2) { return v1.x * v2.x + v1.y * v2.y; }

  [[nodiscard]] T SqrLength() const { return x * x + y * y; }

  [[nodiscard]] T Length() const
    requires std::is_floating_point_v<T>
  {
    return std::sqrt(x * x + y * y);
  }

  Vec2& Normalize() {
    const auto length = Length();
    x /= length;
    y /= length;
    return *this;
  }
};

using Vec2i = Vec2<int>;
using Vec2d = Vec2<double>;

}  // namespace core

#endif  // MATHS_VEC2_H
