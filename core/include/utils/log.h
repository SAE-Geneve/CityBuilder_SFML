#ifndef CORE_UTILS_LOG_H_
#define CORE_UTILS_LOG_H_

#include <format>
#include <print>

namespace core {

template <typename... Args>
void LogDebug([[maybe_unused]] std::format_string<Args...> fmt,
              [[maybe_unused]] Args&&... args) {
#ifndef NDEBUG
  std::print("[DEBUG] {}\n", std::format(fmt, std::forward<Args>(args)...));
#endif
}

template <typename... Args>
void LogWarning(std::format_string<Args...> fmt, Args&&... args) {
  std::print("[WARNING] {}\n", std::format(fmt, std::forward<Args>(args)...));
}

template <typename... Args>
void LogError(std::format_string<Args...> fmt, Args&&... args) {
  std::print(stderr, "[ERROR] {}\n",
             std::format(fmt, std::forward<Args>(args)...));
}

}  // namespace core

#endif  // CORE_UTILS_LOG_H_
