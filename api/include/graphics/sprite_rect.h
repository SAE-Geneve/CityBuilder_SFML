#ifndef API_GRAPHICS_SPRITE_RECT_H_
#define API_GRAPHICS_SPRITE_RECT_H_

namespace api::graphics {

// A single sprite's location inside a spritesheet, plus its pivot/origin.
// Plain aggregate so it can be a compile-time constant emitted by the editor's
// generated headers. Pixel rect is signed per the project's integer guideline;
// origin is in pixels relative to the rect's top-left corner.
struct SpriteRect {
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
  float origin_x = 0.f;
  float origin_y = 0.f;
};

}  // namespace api::graphics

#endif  // API_GRAPHICS_SPRITE_RECT_H_
