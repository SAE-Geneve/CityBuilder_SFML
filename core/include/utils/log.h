#ifndef CITYBUILDER_LOG_H
#define CITYBUILDER_LOG_H

#include <format>
#include <print>

namespace core {

template <typename... Args>
void LogDebug(std::format_string<Args...> fmt, Args&&... args) {
#ifndef NDEBUG
  std::print("[DEBUG] {}\n", std::format(fmt, std::forward<Args>(args)...));
#endif
}

template <typename... Args>
void LogWarning(std::format_string<Args...> fmt, Args&&... args) {
#ifndef NDEBUG
  std::print("[WARNING] {}\n", std::format(fmt, std::forward<Args>(args)...));
#endif
}

template <typename... Args>
void LogError(std::format_string<Args...> fmt, Args&&... args) {
#ifndef NDEBUG
  std::print("[ERROR] {}\n", std::format(fmt, std::forward<Args>(args)...));
#endif
}

}  // namespace core

#endif  // CITYBUILDER_LOG_H
