#ifndef GAME_H
#define GAME_H

namespace game {

struct LaunchOptions {
  int initial_spawn_count = 0;
  int window_width = 1280;
  int window_height = 960;
  int tilemap_width = 20;
  int tilemap_height = 15;
};

void Loop(const LaunchOptions& options);
}  // namespace game

#endif  // GAME_H
