#include "graphics/sprite_sheet.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <filesystem>

#include "profiling/profiling.h"

namespace api::graphics {

// sf::Texture's path constructor throws sf::Exception on failure, matching how
// core::assets::AssetManager loads textures elsewhere in the codebase.
SpriteSheet::SpriteSheet(std::string_view texture_path)
    : texture_(std::filesystem::path(texture_path)), sprite_(texture_) {
  PROFILE_ZONE();
}

void SpriteSheet::Draw(sf::RenderWindow& window, const SpriteRect& rect,
                       sf::Vector2f position) {
  PROFILE_ZONE();
  // Re-bind the texture each draw so the sprite never holds a stale pointer if
  // this SpriteSheet was moved (e.g. stored in a std::vector).
  sprite_.setTexture(texture_, false);
  sprite_.setTextureRect(sf::IntRect({rect.x, rect.y}, {rect.w, rect.h}));
  sprite_.setOrigin({rect.origin_x, rect.origin_y});
  sprite_.setPosition(position);
  window.draw(sprite_);
}

}  // namespace api::graphics
