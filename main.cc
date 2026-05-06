#include "game.h"

#include <argh.h>

int main(int argc, char** argv) {
  argh::parser cmdl;
  cmdl.add_param("init-spawn");
  cmdl.parse(argc, argv);

  game::LaunchOptions options;
  cmdl("init-spawn", 0) >> options.initial_spawn_count;

  game::Loop(options);

  return 0;
}
