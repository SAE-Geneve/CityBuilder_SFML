//
// Created by unite on 07.05.2026.
//

#ifndef CITYBUILDER_STRING_LITERAL_H
#define CITYBUILDER_STRING_LITERAL_H

#include <algorithm>

namespace core::utils {
template <size_t N>
struct StringLiteral {
  constexpr StringLiteral(const char (&arg)[N]) { std::copy_n(arg, N, str); }
  char str[N]{};
};
}
#endif  // CITYBUILDER_STRING_LITERAL_H
