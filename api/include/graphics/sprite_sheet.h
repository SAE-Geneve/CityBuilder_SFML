#ifndef API_GRAPHICS_SPRITE_SHEET_H_
#define API_GRAPHICS_SPRITE_SHEET_H_

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <string_view>

#include "graphics/sprite_rect.h"

namespace sf {
class RenderWindow;
}

namespace api::graphics {

// Owns one spritesheet texture and draws individual sub-sprites from it,
// addressed by the SpriteRect constants emitted into the editor-generated
// headers. Replaces the one-texture-per-sprite AssetManager usage at the call
// sites that draw from a sheet.
class SpriteSheet {
 public:
  explicit SpriteSheet(std::string_view texture_path);

  void Draw(sf::RenderWindow& window, const SpriteRect& rect,
            sf::Vector2f position);

  [[nodiscard]] const sf::Texture& texture() const { return texture_; }

 private:
  sf::Texture texture_;
  // sf::Sprite has no default constructor in SFML 3 (it needs a texture), so it
  // is initialised from texture_ in the constructor and reused per draw.
  sf::Sprite sprite_;
};

}  // namespace api::graphics

#endif  // API_GRAPHICS_SPRITE_SHEET_H_
