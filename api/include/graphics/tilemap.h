#ifndef API_GRAPHICS_TILEMAP_H
#define API_GRAPHICS_TILEMAP_H

#include <SFML/Graphics.hpp>
#include <mdspan>

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

  static constexpr bool IsWalkable(Tile t) noexcept {
    return t != Tile::kWater;
  }

  // Storage element wrapping a Tile so the core pathfinding can call
  // .IsWalkable() on each grid cell directly.
  struct WalkableCell {
    Tile tile = Tile::kEmpty;
    [[nodiscard]] constexpr bool IsWalkable() const noexcept {
      return TileMap::IsWalkable(tile);
    }
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

  // Storage is x-major: flat index = grid_x * tile_count_y_ + grid_y.
  // This matches the layout-right mdspan exposed via AsMdspan().
  std::vector<WalkableCell> tiles_;
  size_t tile_count_x_ = 0;
  size_t tile_count_y_ = 0;
  core::assets::AssetManager<sf::Texture, Tile, "_assets/sprites">
      textures_;


 public:
  sf::Vector2f screen_position(size_t index) const;
  static sf::Vector2f TilePos(sf::Vector2i);
  static constexpr int step(){ return kPixelStep; };

  void Setup(size_t tile_count_x, size_t tile_count_y);
  void Draw(sf::RenderWindow &window);
  void set_tile(size_t, Tile);

  void SetCamera(const sf::RenderWindow &window,
                 const api::graphics::Camera &camera);
  void HandleEvent(std::optional<sf::Event> event, bool &wasClicked) override;

  [[nodiscard]] std::mdspan<const WalkableCell, std::dextents<std::size_t, 2>>
  AsMdspan() const {
    return std::mdspan(tiles_.data(),
                       static_cast<std::size_t>(tile_count_x_),
                       static_cast<std::size_t>(tile_count_y_));
  }

  [[nodiscard]] auto tile_count_x() const { return tile_count_x_; }
  [[nodiscard]] auto tile_count_y() const { return tile_count_y_; }

  std::vector<int> GetCollectibles(Tile) const;
  std::vector<sf::Vector2f> GetWalkables() const;

 private:
  const sf::RenderWindow *window_ = nullptr;
  const api::graphics::Camera *camera_ = nullptr;
};
#endif
