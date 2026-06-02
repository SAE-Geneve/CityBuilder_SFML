#include "grid_slicer.h"

#include <format>

namespace sprite_editor {

std::vector<EditorSprite> GenerateGrid(const GridParams& params,
                                       int texture_w, int texture_h,
                                       std::string_view name_prefix) {
  std::vector<EditorSprite> sprites;
  if (params.cell_w <= 0 || params.cell_h <= 0) {
    return sprites;
  }

  const int limit_x = texture_w - params.margin_x;
  const int limit_y = texture_h - params.margin_y;
  const int step_x = params.cell_w + params.spacing_x;
  const int step_y = params.cell_h + params.spacing_y;

  int index = 0;
  for (int y = params.offset_y; y + params.cell_h <= limit_y; y += step_y) {
    for (int x = params.offset_x; x + params.cell_w <= limit_x; x += step_x) {
      EditorSprite sprite;
      sprite.name = std::format("{}{}", name_prefix, index);
      sprite.x = x;
      sprite.y = y;
      sprite.w = params.cell_w;
      sprite.h = params.cell_h;
      sprite.origin_x = static_cast<float>(params.cell_w) / 2.f;
      sprite.origin_y = static_cast<float>(params.cell_h) / 2.f;
      sprites.push_back(std::move(sprite));
      ++index;
    }
  }

  return sprites;
}

}  // namespace sprite_editor
