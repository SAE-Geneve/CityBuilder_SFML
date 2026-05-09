#ifndef GAME_H
#define GAME_H


#include <cstdlib>
namespace game {

struct LaunchOptions {
  size_t initial_spawn_count = 0;
  int window_width = 1280;
  int window_height = 960;
  size_t tilemap_width = 20;
  size_t tilemap_height = 15;
};

void Loop(const LaunchOptions& options);
}  // namespace game

#endif  // GAME_H
