#include "graphics/tilemap.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <random>
#include <ranges>

#include "FastNoiseLite.h"
#include "graphics/camera.h"
#include "profiling/profiling.h"

static std::mt19937 gen{std::random_device{}()};
static std::uniform_real_distribution dist(0.f, 1.f);

void TileMap::Setup(int tile_count_x, int tile_count_y) {
  PROFILE_ZONE();
  tile_count_x_ = tile_count_x;
  tile_count_y_ = tile_count_y;
  tiles_.assign(static_cast<std::size_t>(tile_count_x_ * tile_count_y_),
                Tile::kBg);

  // Create and configure FastNoise object
  FastNoiseLite noise;
  noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
  noise.SetSeed(1309);
  noise.SetFrequency(0.01f);

  textures_.Load(files_);

  std::array weights = {std::pair{Tile::kWater, 3.f},
                        std::pair{Tile::kRock, 1.f},
                        std::pair{Tile::kTree, 1.f},
                        std::pair{Tile::kFood, 1.f}, std::pair{Tile::kBg, 6.f}};

  // Remplir le tableau avec de l'herbe ou de l'eau de manière aléatoire

  // Fixed, why were weights sorted for each tile when not modified?
  std::ranges::sort(weights,
            [](auto &a, auto &b) { return a.second < b.second; });
  for (int tileIndex = 0; tileIndex < std::ssize(tiles_); ++tileIndex) {
    sf::Vector2f pos = ScreenPosition(tileIndex);

    // auto value = 0.5f * (noise.GetNoise(pos.x, pos.y) + 1);
    auto value = dist(gen);

    float sumWeight = 0;
    for (const auto &weight : weights | std::views::values) {
      sumWeight += weight;
    }

    float localWeight = 0;
    Tile goodTile = Tile::kBg;

    for (auto &[tile, weight] : weights) {
      localWeight += weight;

      if ((value * sumWeight) < localWeight) {
        goodTile = tile;
        break;
      }
    }

    tiles_[tileIndex] = goodTile;
    if (goodTile != Tile::kWater) {
      walkables_.push_back(pos);
    }
  }

  SetZone(sf::IntRect({0, 0}, sf::Vector2i(tile_count_x_ * kPixelStep,
                                           tile_count_y_ * kPixelStep)));
}

void TileMap::Draw(sf::RenderWindow &window) {
  PROFILE_ZONE();
  int tileIndex = 0;

  //FIXME use sf::VertexArray instead of sf::Sprite
  sf::Sprite sprite(textures_.Get(Tile::kEmpty));

  sprite.setTextureRect(sf::IntRect({0, 0}, {kPixelStep, kPixelStep}));

  for (auto tile : tiles_) {
    sprite.setTexture(textures_.Get(tile));
    sprite.setPosition(ScreenPosition(tileIndex));
    window.draw(sprite);

    tileIndex++;
  }
}

void TileMap::SetTile(int idx, Tile tile) {
  PROFILE_ZONE();
  if (idx > 0 && idx < std::ssize(tiles_)) {
    tiles_[idx] = tile;
  }
}

const std::vector<sf::Vector2f> &TileMap::GetWalkables() const {
  return walkables_;
}

std::vector<int> TileMap::GetCollectibles(Tile search_tile) const {
  PROFILE_ZONE();
  std::vector<int> collectibles;

  for (int tile_index = 0; tile_index < std::ssize(tiles_); ++tile_index) {
    if (tiles_[tile_index] == search_tile) {
      collectibles.emplace_back(tile_index);
    }
  }

  return collectibles;
}

sf::Vector2f TileMap::ScreenPosition(const int index) const {
  float x = static_cast<float>((index % tile_count_x_) * kPixelStep);
  float y = static_cast<float>((index / tile_count_x_) * kPixelStep);
  return {x, y};
}

int TileMap::Index(const sf::Vector2f screenPosition) const {
  return static_cast<int>(ceil(screenPosition.y / kPixelStep)) * tile_count_x_ +
         static_cast<int>(ceil(screenPosition.x / kPixelStep));
}

sf::Vector2f TileMap::TilePos(sf::Vector2i pos) {
  return {static_cast<float>(round(pos.x / kPixelStep) * kPixelStep),
          static_cast<float>(round(pos.y / kPixelStep) * kPixelStep)};
}

void TileMap::SetCamera(const sf::RenderWindow &window,
                        const api::graphics::Camera &camera) {
  window_ = &window;
  camera_ = &camera;
}

void TileMap::HandleEvent(std::optional<sf::Event> event, bool &wasClicked) {
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

  if (const auto *p = event->getIf<sf::Event::MouseButtonPressed>()) {
    sf::Event remapped{
        sf::Event::MouseButtonPressed{p->button, remap(p->position)}};
    api::ui::Clickable::HandleEvent(remapped, wasClicked);
    return;
  }
  if (const auto *r = event->getIf<sf::Event::MouseButtonReleased>()) {
    sf::Event remapped{
        sf::Event::MouseButtonReleased{r->button, remap(r->position)}};
    api::ui::Clickable::HandleEvent(remapped, wasClicked);
    return;
  }
  if (const auto *m = event->getIf<sf::Event::MouseMoved>()) {
    sf::Event remapped{sf::Event::MouseMoved{remap(m->position)}};
    api::ui::Clickable::HandleEvent(remapped, wasClicked);
    return;
  }

  api::ui::Clickable::HandleEvent(event, wasClicked);
}
