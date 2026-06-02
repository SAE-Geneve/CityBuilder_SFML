#ifndef TOOLS_SPRITE_EDITOR_GRID_SLICER_H_
#define TOOLS_SPRITE_EDITOR_GRID_SLICER_H_

#include <string_view>
#include <vector>

#include "sheet_document.h"

namespace sprite_editor {

// Parameters describing a uniform grid over a spritesheet. `offset` is the
// top-left pixel of the first cell, `margin` is reserved space on the right and
// bottom edges, `spacing` is the gap between adjacent cells.
struct GridParams {
  int cell_w = 64;
  int cell_h = 64;
  int offset_x = 0;
  int offset_y = 0;
  int spacing_x = 0;
  int spacing_y = 0;
  int margin_x = 0;
  int margin_y = 0;
};

// Generates one EditorSprite per fully-contained grid cell, scanning left to
// right then top to bottom. Each sprite is named "<prefix><index>" and its
// origin defaults to the cell centre. Returns empty if the grid is degenerate.
std::vector<EditorSprite> GenerateGrid(const GridParams& params,
                                       int texture_w, int texture_h,
                                       std::string_view name_prefix);

}  // namespace sprite_editor

#endif  // TOOLS_SPRITE_EDITOR_GRID_SLICER_H_
