#ifndef CORE_UTILS_STRING_LITERAL_H_
#define CORE_UTILS_STRING_LITERAL_H_

#include <algorithm>

namespace core::utils {
template <size_t N>
struct StringLiteral {
  constexpr StringLiteral(const char (&arg)[N]) { std::copy_n(arg, N, str); }
  char str[N]{};
};
}
#endif  // CORE_UTILS_STRING_LITERAL_H_
