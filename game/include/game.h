#ifndef GAME_H
#define GAME_H

namespace game {

struct LaunchOptions {
  int initial_spawn_count = 0;
};

void Loop(const LaunchOptions& options);
}  // namespace game

#endif  // GAME_H
