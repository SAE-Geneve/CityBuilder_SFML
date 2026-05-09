#include <gtest/gtest.h>

#include <SFML/Graphics/Texture.hpp>

#include "assets/asset_manager.h"

TEST(TextureManager, Load) {
  enum class TextureIndex { kDefault = 0, KGrass, KMaison, kLength };
  using TextureManager =
      core::assets::AssetManager<sf::Texture, TextureIndex,
                                       "../_assets/sprites/">;
  static constexpr std::array<std::string_view,
                              static_cast<int>(TextureIndex::kLength)>
      paths{{
          "empty.png",
          "grass.png",
          "maison.png",
      }};
  TextureManager textureManager;
  textureManager.Load(paths);

  [[maybe_unused]] auto& maisonTexture = textureManager.Get(TextureIndex::KMaison);

}