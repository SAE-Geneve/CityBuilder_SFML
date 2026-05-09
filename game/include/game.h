#ifndef GAME_GAME_H_
#define GAME_GAME_H_

#include <cstdint>
#include <cstdlib>

namespace game {

struct LaunchOptions {
  size_t initial_spawn_count = 0;
  uint32_t window_width = 1280;
  uint32_t window_height = 960;
  size_t tilemap_width = 20;
  size_t tilemap_height = 15;
};

void Loop(const LaunchOptions& options);
}  // namespace game

#endif  // GAME_GAME_H_
