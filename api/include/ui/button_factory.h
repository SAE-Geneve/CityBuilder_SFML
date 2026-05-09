#ifndef BUTTON_FACTORY_H
#define BUTTON_FACTORY_H

#include <SFML/Graphics/Font.hpp>
#include <array>
#include <string_view>

#include "assets/asset_manager.h"
#include "button.h"
#include "profiling/profiling.h"
#include "utils/log.h"

namespace api::ui {
class ButtonFactory {
  enum class Sprite { kEmpty, kBgBtn, kHoverBtn, kLength };

  std::array<std::string_view, static_cast<size_t>(Sprite::kLength)> files_ = {
      "empty.png", "button_grey.png", "button_brown.png"};

  sf::Font font;
  core::assets::AssetManager<sf::Texture, Sprite, "_assets/sprites">
      textures_;

 public:
  ButtonFactory() {
    PROFILE_ZONE();
    textures_.Load(files_);
    if (!font.openFromFile("_assets/fonts/ANTQUAB.TTF")) {
      core::LogError("Failed to load font");
    }
  }

  std::unique_ptr<Button> CreateButton(sf::Vector2f pos,
                                       std::string_view label) {
    PROFILE_ZONE();
    return std::make_unique<api::ui::Button>(
        pos, label, textures_.Get(Sprite::kBgBtn),
        textures_.Get(Sprite::kHoverBtn), font);
  }
};
}  // namespace api::ui

#endif  // BUTTON_FACTORY_H
