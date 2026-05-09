#ifndef API_UI_BUTTON_FACTORY_H_
#define API_UI_BUTTON_FACTORY_H_

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

  sf::Font font_;
  core::assets::AssetManager<sf::Texture, Sprite, "_assets/sprites">
      textures_;

 public:
  ButtonFactory() {
    PROFILE_ZONE();
    textures_.Load(files_);
    if (!font_.openFromFile("_assets/fonts/ANTQUAB.TTF")) {
      core::LogError("Failed to load font");
    }
  }

  std::unique_ptr<Button> CreateButton(sf::Vector2f pos,
                                       std::string_view label) {
    PROFILE_ZONE();
    return std::make_unique<api::ui::Button>(
        pos, label, textures_.Get(Sprite::kBgBtn),
        textures_.Get(Sprite::kHoverBtn), font_);
  }
};
}  // namespace api::ui

#endif  // API_UI_BUTTON_FACTORY_H_
