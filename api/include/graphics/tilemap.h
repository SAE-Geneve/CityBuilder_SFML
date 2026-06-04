#ifndef API_GRAPHICS_TILEMAP_H_
#define API_GRAPHICS_TILEMAP_H_

// TODO(google-style): narrow this umbrella include to the specific SFML
// headers actually used by this header.
#include <SFML/Graphics.hpp>
#include <array>
#include <mdspan>
#include <string_view>
#include <vector>

#include "sprites/bg_plus_food.generated.h"
#include "sprites/bg_plus_rock.generated.h"
#include "sprites/bg_plus_tree.generated.h"
#include "sprites/bg_tile_a.generated.h"
#include "sprites/empty.generated.h"
#include "sprites/scifienvironment_15.generated.h"
#include "sprites/water.generated.h"
#include "graphics/sprite_sheet.h"
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
  // Each tile is drawn from its own single-sprite spritesheet, addressed by the
  // SpriteRect constants emitted into the editor-generated headers. Indexed by
  // the Tile enum (same order as the enum declaration).
  static constexpr std::array<api::graphics::SpriteRect,
                              static_cast<size_t>(Tile::kLength)>
      kTileRects = {api::graphics::sprites::empty::kFull,
                    api::graphics::sprites::bg_tile_a::kFull,
                    api::graphics::sprites::water::kFull,
                    api::graphics::sprites::bg_plus_rock::kFull,
                    api::graphics::sprites::bg_plus_tree::kFull,
                    api::graphics::sprites::bg_plus_food::kFull,
                    api::graphics::sprites::scifienvironment_15::kFull};

  static constexpr std::array<std::string_view,
                              static_cast<size_t>(Tile::kLength)>
      kTileTextures = {api::graphics::sprites::empty::kTexture,
                       api::graphics::sprites::bg_tile_a::kTexture,
                       api::graphics::sprites::water::kTexture,
                       api::graphics::sprites::bg_plus_rock::kTexture,
                       api::graphics::sprites::bg_plus_tree::kTexture,
                       api::graphics::sprites::bg_plus_food::kTexture,
                       api::graphics::sprites::scifienvironment_15::kTexture};

  // Storage is x-major: flat index = grid_x * tile_count_y_ + grid_y.
  // This matches the layout-right mdspan exposed via AsMdspan().
  std::vector<WalkableCell> tiles_;
  size_t tile_count_x_ = 0;
  size_t tile_count_y_ = 0;
  std::vector<api::graphics::SpriteSheet> tile_sheets_;


 public:
  sf::Vector2f screen_position(size_t index) const;
  static sf::Vector2f tile_pos(sf::Vector2i);
  static constexpr int step(){ return kPixelStep; };

  void Setup(size_t tile_count_x, size_t tile_count_y);
  void Draw(sf::RenderWindow& window);
  void set_tile(size_t, Tile);

  void set_camera(const sf::RenderWindow& window,
                 const api::graphics::Camera& camera);
  void HandleEvent(std::optional<sf::Event> event, bool& wasClicked) override;

  [[nodiscard]] std::mdspan<const WalkableCell, std::dextents<std::size_t, 2>>
  AsMdspan() const {
    return std::mdspan(tiles_.data(),
                       static_cast<std::size_t>(tile_count_x_),
                       static_cast<std::size_t>(tile_count_y_));
  }

  [[nodiscard]] auto tile_count_x() const { return tile_count_x_; }
  [[nodiscard]] auto tile_count_y() const { return tile_count_y_; }

  std::vector<int> collectibles(Tile) const;
  std::vector<sf::Vector2f> walkables() const;

 private:
  const sf::RenderWindow* window_ = nullptr;
  const api::graphics::Camera* camera_ = nullptr;
};
#endif  // API_GRAPHICS_TILEMAP_H_
