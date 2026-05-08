#ifndef API_GRAPHICS_TILEMAP_H
#define API_GRAPHICS_TILEMAP_H

#include <SFML/Graphics.hpp>

#include "assets/asset_manager.h"
#include "ui/clickable.h"

namespace api::graphics {
class Camera;
}

class TileMap : public api::ui::Clickable {
 public:
  static constexpr int kPixelStep = 64;

  enum class Tile {
    kEmpty,
    kBg,
    kWater,
    kRock,
    kTree,
    kFood,
    kCutTree,
    kLength
  };

 private:
  std::array<std::string_view, static_cast<size_t>(Tile::kLength)> files_ = {
      "empty.png",
      "bg_tile_a.png",
      "water.png",
      "bg_plus_rock.png",
      "bg_plus_tree.png",
      "bg_plus_food.png",
      "scifiEnvironment_15.png"};

  std::vector<Tile> tiles_;
  int tile_count_x_ = 0;
  int tile_count_y_ = 0;
  core::assets::AssetManager<sf::Texture, Tile, "_assets/sprites">
      textures_;

  std::vector<sf::Vector2f> walkables_;

  int Index(sf::Vector2f screenPosition) const;

 public:
  sf::Vector2f ScreenPosition(int index) const;
  static sf::Vector2f TilePos(sf::Vector2i);
  static int GetStep() { return kPixelStep; };

  void Setup(int tile_count_x, int tile_count_y);
  void Draw(sf::RenderWindow &window);
  void SetTile(int, Tile);

  void SetCamera(const sf::RenderWindow &window,
                 const api::graphics::Camera &camera);
  void HandleEvent(std::optional<sf::Event> event, bool &wasClicked) override;

  const std::vector<sf::Vector2f> &GetWalkables() const;

  std::vector<int> GetCollectibles(Tile) const;

 private:
  const sf::RenderWindow *window_ = nullptr;
  const api::graphics::Camera *camera_ = nullptr;
};
#endif
