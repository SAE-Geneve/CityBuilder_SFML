#include "game.h"

#include <array>
#include <random>

#include "SFML/Graphics.hpp"
#include "ai/npc_manager.h"
#include "graphics/camera.h"
#include "graphics/tilemap.h"
#include "maths/vec2.h"
#include "profiling/profiling.h"
#include "resources/resource_manager.h"
#include "ui/button.h"
#include "ui/button_factory.h"
#include "ui/clickable.h"
#include "utils/log.h"

namespace game {
namespace {
sf::Clock clock;
sf::RenderWindow window_;

auto tilemap_ptr_ = std::make_unique<TileMap>();
api::ai::NpcManager npc_manager_;
api::graphics::Camera camera_;

// UI Elements
api::ui::ButtonFactory btn_factory;

std::unique_ptr<api::ui::Button> btnBlue;
std::unique_ptr<api::ui::Button> btnRed;
std::unique_ptr<api::ui::Button> btnGreen;
std::unique_ptr<api::ui::Button> btnExit;

api::ai::NpcType npc_adding_type = api::ai::NpcType::kNone;

api::resource::ResourceManager resource_manager;

void ChopEvent(int index, float quantity) {
  PROFILE_ZONE();
  if (quantity <= 0) {
    tilemap_ptr_->set_tile(static_cast<size_t>(index), TileMap::Tile::kBg);
  }
}

void Setup(const LaunchOptions& options) {
  PROFILE_ZONE();
  // Create the main window
  window_.create(sf::VideoMode({static_cast<unsigned>(options.window_width),
                                static_cast<unsigned>(options.window_height)}),
                 "SFML window");

  camera_.Setup(window_.getSize());

  tilemap_ptr_->Setup(options.tilemap_width, options.tilemap_height);
  tilemap_ptr_->SetCamera(window_, camera_);
  tilemap_ptr_->on_released_left = []() {
    core::LogDebug("Clicked tilemap");
    const sf::Vector2f world = window_.mapPixelToCoords(
        sf::Mouse::getPosition(window_), camera_.WorldView());
    npc_manager_.Add(npc_adding_type, TileMap::TilePos(sf::Vector2i(world)),
                     tilemap_ptr_.get(), resource_manager);
    npc_adding_type = api::ai::NpcType::kNone;
  };

  const auto window_size = core::maths::Vec2f{window_.getSize()};
  btnBlue = btn_factory.CreateButton(
      sf::Vector2f(100.f, window_size.y - 100.f), "Blue");
  btnBlue->on_released_left = []() {
    npc_adding_type = api::ai::NpcType::kBlueWood;
  };

  btnRed = btn_factory.CreateButton(
      sf::Vector2f(200.f, window_size.y - 100.f), "Red");
  btnRed->on_released_left = []() {
    npc_adding_type = api::ai::NpcType::kRedRock;
  };

  btnGreen = btn_factory.CreateButton(
      sf::Vector2f(300.f, window_size.y - 100.f), "Green");
  btnGreen->on_released_left = []() {
    npc_adding_type = api::ai::NpcType::kGreenFood;
  };

  btnExit = btn_factory.CreateButton(
      sf::Vector2f(window_size.y - 30.f, 30.f), "Exit");
  btnExit->on_released_left = []() { window_.close(); };

  resource_manager.LoadResources(
      api::resource::Resource::Type::kWood,
      tilemap_ptr_->GetCollectibles(TileMap::Tile::kTree), ChopEvent);

  resource_manager.LoadResources(
      api::resource::Resource::Type::kFood,
      tilemap_ptr_->GetCollectibles(TileMap::Tile::kFood), ChopEvent);

  resource_manager.LoadResources(
      api::resource::Resource::Type::kStone,
      tilemap_ptr_->GetCollectibles(TileMap::Tile::kRock), ChopEvent);
}

void SpawnInitialNpcs(size_t total) {
  PROFILE_ZONE();
  if (total <= 0) {
    return;
  }

  const auto& walkables = tilemap_ptr_->GetWalkables();
  if (walkables.empty()) {
    core::LogWarning("No walkable tiles available for initial NPC spawn");
    return;
  }

  static std::mt19937 gen{std::random_device{}()};
  std::uniform_int_distribution<size_t> tile_dist(
      0, std::size(walkables) - 1);

  constexpr std::array<api::ai::NpcType, 3> kTypes = {
      api::ai::NpcType::kBlueWood,
      api::ai::NpcType::kRedRock,
      api::ai::NpcType::kGreenFood,
  };

  const auto base = total / 3;
  const auto remainder = total % 3;
  for (size_t t = 0; t < std::size(kTypes); ++t) {
    const auto count = base + (t < remainder ? 1 : 0);
    for (size_t i = 0; i < count; ++i) {
      const auto random_index = tile_dist(gen);
      const auto& position = walkables[random_index];
      npc_manager_.Add(kTypes[t], position, tilemap_ptr_.get(),
                       resource_manager);
    }
  }
}
}  // namespace

void Loop(const LaunchOptions& options) {
  Setup(options);
  SpawnInitialNpcs(options.initial_spawn_count);

  // Start the game loop
  while (window_.isOpen()) {
    auto dt = clock.restart().asSeconds();

    // Process events = Input frame
    while (const std::optional event = window_.pollEvent()) {
      // Close window: exit
      if (event->is<sf::Event::Closed>()) {
        window_.close();
      }

      bool buttonsWasClicked = false;
      btnBlue->HandleEvent(event, buttonsWasClicked);
      btnRed->HandleEvent(event, buttonsWasClicked);
      btnGreen->HandleEvent(event, buttonsWasClicked);
      btnExit->HandleEvent(event, buttonsWasClicked);

      camera_.HandleEvent(*event, window_);

      tilemap_ptr_->HandleEvent(event, buttonsWasClicked);
    }

    // GamePlay, physic frame
    camera_.Update(dt);
    npc_manager_.Update(dt);

    PROFILE_PLOT("Frame dt (ms)", dt * 1000.0f);
    PROFILE_PLOT("NPC count", static_cast<int64_t>(npc_manager_.Count()));

    // Graphic frame
    window_.clear();

    camera_.Apply(window_);
    tilemap_ptr_->Draw(window_);
    npc_manager_.Draw(window_);

    camera_.ApplyUi(window_);
    btnBlue->Draw(window_);
    btnRed->Draw(window_);
    btnGreen->Draw(window_);
    btnExit->Draw(window_);

    window_.display();
    PROFILE_FRAME();
  }
}
}  // namespace game
