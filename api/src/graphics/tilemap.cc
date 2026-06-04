#include "graphics/tilemap.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <algorithm>
#include <cmath>
#include <array>
#include <format>
#include <random>

#include "FastNoiseLite.h"
#include "graphics/camera.h"
#include "profiling/profiling.h"

namespace {
namespace sci = api::graphics::sprites::scifirts_spritesheet;

// TODO(google-style): file-scope statics with non-trivial destructors. Move
// behind a function returning a reference to a function-local static.
std::mt19937 gen{std::random_device{}()};
std::uniform_real_distribution dist(0.f, 1.f);

// Per-biome sprite variants; one is picked at random for each generated tile.
constexpr std::array kIceVariants{sci::kIce1, sci::kIce2};
constexpr std::array kEmptyVariants{sci::kEmpty1, sci::kEmpty2};
constexpr std::array kForestVariants{sci::kForest1, sci::kForest2, sci::kForest3,
                                     sci::kForest4};
constexpr std::array kRoundForestVariants{sci::kRoundForest1, sci::kRoundForest2,
                                          sci::kRoundForest3, sci::kRoundForest4};

// Gatherable resources drawn from the same atlas; one variant is picked per tile.
constexpr std::array kStoneVariants{sci::kGreyStone0, sci::kGreyStone1,
                                    sci::kGreyStone2, sci::kGreyStone3};
constexpr std::array kCrystalVariants{sci::kGreenCrystal1, sci::kGreenCrystal2};

// Noise frequency in cycles per tile. ~1/freq tiles per biome blob, so this is
// tuned so several biomes are visible across the default ~20-tile-wide map.
constexpr float kNoiseFrequency = 0.08f;

// Biome selection thresholds for the two noise fields (Perlin output ~[-1, 1]).
constexpr float kColdThreshold = -0.3f;    // temperature below this -> ice
constexpr float kDenseThreshold = 0.4f;    // moisture above this -> round forest
constexpr float kForestThreshold = 0.0f;   // moisture above this -> forest

// Fraction of empty tiles seeded with a gatherable resource so NPC behaviour
// trees still have collectibles to target.
constexpr float kResourceChance = 0.06f;

template <std::size_t N>
const api::graphics::SpriteRect& PickVariant(
    const std::array<api::graphics::SpriteRect, N>& variants) {
  const auto i = static_cast<std::size_t>(dist(gen) * static_cast<float>(N));
  return variants[std::min(i, N - 1)];
}
}  // namespace

void TileMap::Setup(size_t tile_count_x, size_t tile_count_y) {
  PROFILE_ZONE();
  tile_count_x_ = tile_count_x;
  tile_count_y_ = tile_count_y;
  tiles_.assign(tile_count_x_ * tile_count_y_,
                WalkableCell{Tile::kEmpty, sci::kEmpty1});

  // Two independent noise fields drive the biome at each tile: "temperature"
  // selects ice, "moisture" selects the forest density.
  FastNoiseLite temperature;
  temperature.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
  temperature.SetSeed(1309);
  temperature.SetFrequency(kNoiseFrequency);

  FastNoiseLite moisture;
  moisture.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
  moisture.SetSeed(4231);
  moisture.SetFrequency(kNoiseFrequency);

  tile_sheets_.clear();
  tile_sheets_.reserve(kTileTextures.size());
  for (const auto& texture : kTileTextures) {
    tile_sheets_.emplace_back(std::format("_assets/sprites/{}", texture));
  }

  // Storage is x-major: flat index = grid_x * tile_count_y_ + grid_y.
  for (size_t gx = 0; gx < tile_count_x_; ++gx) {
    for (size_t gy = 0; gy < tile_count_y_; ++gy) {
      const auto fx = static_cast<float>(gx);
      const auto fy = static_cast<float>(gy);
      const float t = temperature.GetNoise(fx, fy);
      const float m = moisture.GetNoise(fx, fy);

      WalkableCell cell;
      if (t < kColdThreshold) {
        cell = {Tile::kIce, PickVariant(kIceVariants)};
      } else if (m > kDenseThreshold) {
        cell = {Tile::kRoundForest, PickVariant(kRoundForestVariants)};
      } else if (m > kForestThreshold) {
        cell = {Tile::kForest, PickVariant(kForestVariants)};
      } else {
        cell = {Tile::kEmpty, PickVariant(kEmptyVariants)};
        // Sprinkle gatherable resources onto open ground so NPCs have targets.
        // Trees are not seeded here: wood NPCs gather from the forest biomes.
        if (dist(gen) < kResourceChance) {
          if (dist(gen) < 0.5f) {
            cell = {Tile::kRock, PickVariant(kStoneVariants)};
          } else {
            cell = {Tile::kFood, PickVariant(kCrystalVariants)};
          }
        }
      }
      tiles_[gx * tile_count_y_ + gy] = cell;
    }
  }

  SetZone(sf::IntRect({0, 0}, sf::Vector2i(static_cast<int>(tile_count_x_) * kPixelStep,
                                           static_cast<int>(tile_count_y_) * kPixelStep)));
}

void TileMap::Draw(sf::RenderWindow& window) {
  PROFILE_ZONE();

  // Draws one atlas sprite centered inside the cell at `screen`, then re-applies
  // the rect origin (which SpriteSheet::Draw subtracts). Sub-cell sprites (stone,
  // crystal) end up centered; full 64x64 tiles get a zero offset and so are
  // unaffected, regardless of their origin convention.
  const auto draw_in_cell = [this, &window](size_t sheet,
                                            const api::graphics::SpriteRect& rect,
                                            sf::Vector2f screen) {
    auto position = screen;
    position += sf::Vector2f(static_cast<float>(kPixelStep - rect.w) * 0.5f,
                             static_cast<float>(kPixelStep - rect.h) * 0.5f);
    position += sf::Vector2f(rect.origin_x, rect.origin_y);
    tile_sheets_[sheet].Draw(window, rect, position);
  };

  size_t tile_index = 0;
  // FIXME use sf::VertexArray instead of per-tile SpriteSheet draws
  for (auto cell : tiles_) {
    const auto screen = screen_position(tile_index);
    // Sub-cell resource sprites don't cover their cell, so paint empty ground
    // behind them first.
    if (cell.tile == Tile::kRock || cell.tile == Tile::kFood) {
      constexpr auto kEmptyIndex = static_cast<size_t>(Tile::kEmpty);
      draw_in_cell(kEmptyIndex, kTileRects[kEmptyIndex], screen);
    }
    draw_in_cell(static_cast<size_t>(cell.tile), cell.rect, screen);
    tile_index++;
  }
}

void TileMap::set_tile(size_t idx, Tile tile) {
  PROFILE_ZONE();
  if (idx > 0 && idx < std::size(tiles_)) {
    tiles_[idx] = WalkableCell{tile, kTileRects[static_cast<size_t>(tile)]};
  }
}

std::vector<sf::Vector2f> TileMap::walkables() const {
  PROFILE_ZONE();
  std::vector<sf::Vector2f> walkables;
  for (size_t tile_index = 0; tile_index < std::size(tiles_); ++tile_index) {
    if (tiles_[tile_index].IsWalkable()) {
      walkables.push_back(screen_position(tile_index));
    }
  }
  return walkables;
}

std::vector<int> TileMap::collectibles(Tile search_tile) const {
  PROFILE_ZONE();
  std::vector<int> collectibles;

  for (size_t tile_index = 0; tile_index < std::size(tiles_); ++tile_index) {
    if (tiles_[tile_index].tile == search_tile) {
      collectibles.emplace_back(static_cast<int>(tile_index));
    }
  }

  return collectibles;
}

sf::Vector2f TileMap::screen_position(size_t index) const {
  // Storage is x-major: flat index = x * tile_count_y_ + y.
  const float x = static_cast<float>(index / tile_count_y_ * kPixelStep);
  const float y = static_cast<float>(index % tile_count_y_ * kPixelStep);
  return {x, y};
}


sf::Vector2f TileMap::tile_pos(sf::Vector2i pos) {
  return {static_cast<float>(std::round(pos.x / kPixelStep) * kPixelStep),
          static_cast<float>(std::round(pos.y / kPixelStep) * kPixelStep)};
}

void TileMap::set_camera(const sf::RenderWindow& window,
                        const api::graphics::Camera& camera) {
  window_ = &window;
  camera_ = &camera;
}

void TileMap::HandleEvent(std::optional<sf::Event> event, bool& wasClicked) {
  PROFILE_ZONE();
  if (!event || !window_ || !camera_) {
    api::ui::Clickable::HandleEvent(event, wasClicked);
    return;
  }

  const auto remap = [this](sf::Vector2i pixel) -> sf::Vector2i {
    const sf::Vector2f world =
        window_->mapPixelToCoords(pixel, camera_->WorldView());
    return sf::Vector2i(static_cast<int>(world.x), static_cast<int>(world.y));
  };

  if (const auto* p = event->getIf<sf::Event::MouseButtonPressed>()) {
    sf::Event remapped{
        sf::Event::MouseButtonPressed{p->button, remap(p->position)}};
    api::ui::Clickable::HandleEvent(remapped, wasClicked);
    return;
  }
  if (const auto* r = event->getIf<sf::Event::MouseButtonReleased>()) {
    sf::Event remapped{
        sf::Event::MouseButtonReleased{r->button, remap(r->position)}};
    api::ui::Clickable::HandleEvent(remapped, wasClicked);
    return;
  }
  if (const auto* m = event->getIf<sf::Event::MouseMoved>()) {
    sf::Event remapped{sf::Event::MouseMoved{remap(m->position)}};
    api::ui::Clickable::HandleEvent(remapped, wasClicked);
    return;
  }

  api::ui::Clickable::HandleEvent(event, wasClicked);
}
