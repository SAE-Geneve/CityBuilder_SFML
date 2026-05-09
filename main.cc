#include "game.h"
#include "utils/log.h"

#include <argh.h>

namespace {
void ApplyPositive(int& field, int parsed, const char* flag) {
  if (parsed <= 0) {
    core::LogWarning("Invalid value for --{} ({}); using default {}", flag,
                     parsed, field);
    return;
  }
  field = parsed;
}
void ApplyPositive(size_t& field, size_t parsed, const char* flag) {
  if (parsed <= 0) {
    core::LogWarning("Invalid value for --{} ({}); using default {}", flag,
                     parsed, field);
    return;
  }
  field = parsed;
}
}  // namespace

int main(int argc, char** argv) {
  argh::parser cmdl;
  cmdl.add_params({"init-spawn", "window-width", "window-height",
                   "tilemap-width", "tilemap-height"});
  cmdl.parse(argc, argv);

  game::LaunchOptions options;
  cmdl("init-spawn", 0) >> options.initial_spawn_count;

  int parsed = 0;
  cmdl("window-width", options.window_width) >> parsed;
  ApplyPositive(options.window_width, parsed, "window-width");
  cmdl("window-height", options.window_height) >> parsed;
  ApplyPositive(options.window_height, parsed, "window-height");
  size_t parsed2 = 0;
  cmdl("tilemap-width", options.tilemap_width) >> parsed2;
  ApplyPositive(options.tilemap_width, parsed2, "tilemap-width");
  cmdl("tilemap-height", options.tilemap_height) >> parsed;
  ApplyPositive(options.tilemap_height, parsed2, "tilemap-height");

  game::Loop(options);

  return 0;
}
