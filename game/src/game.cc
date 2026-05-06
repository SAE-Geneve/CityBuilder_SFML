#include "game.h"

#include <array>
#include <random>

#include "SFML/Graphics.hpp"
#include "utils/log.h"
#include "ai/npc_manager.h"
#include "graphics/tilemap.h"
#include "resources/resource_manager.h"
#include "ui/button.h"
#include "ui/button_factory.h"
#include "ui/clickable.h"

#include "profiling/profiling.h"

namespace game {
namespace {
sf::Clock clock;
sf::RenderWindow window_;

auto tilemap_ptr_ = std::make_unique<TileMap>();
api::ai::NpcManager npc_manager_;

// UI Elements
api::ui::ButtonFactory btn_factory;

std::unique_ptr<api::ui::Button> btnBlue;
std::unique_ptr<api::ui::Button> btnRed;
std::unique_ptr<api::ui::Button> btnGreen;
std::unique_ptr<api::ui::Button> btnExit;

api::ai::NpcType npc_adding_type = api::ai::NpcType::kNone;

ResourceManager resource_manager;

void ChopEvent(int index, float quantity) {
  PROFILE_ZONE();
  if (quantity <= 0) {
    tilemap_ptr_->SetTile(index, TileMap::Tile::kBg);
  }
}

void Setup() {
  PROFILE_ZONE();
  // Create the main window
  window_.create(sf::VideoMode({1280, 1080}), "SFML window");

  tilemap_ptr_->Setup();
  tilemap_ptr_->OnReleasedLeft = []() {
    core::LogDebug("Clicked tilemap");
    npc_manager_.Add(npc_adding_type,
                     TileMap::TilePos(sf::Mouse::getPosition(window_)),
                     tilemap_ptr_.get(), resource_manager);
    npc_adding_type = api::ai::NpcType::kNone;
  };

  btnBlue = btn_factory.CreateButton(
      sf::Vector2f(100.f, window_.getSize().y - 100.f), "Blue");
  btnBlue->OnReleasedLeft = []() {
    npc_adding_type = api::ai::NpcType::kBlueWood;
  };

  btnRed = btn_factory.CreateButton(
      sf::Vector2f(200.f, window_.getSize().y - 100.f), "Red");
  btnRed->OnReleasedLeft = []() {
    npc_adding_type = api::ai::NpcType::kRedRock;
  };

  btnGreen = btn_factory.CreateButton(
      sf::Vector2f(300.f, window_.getSize().y - 100.f), "Green");
  btnGreen->OnReleasedLeft = []() {
    npc_adding_type = api::ai::NpcType::kGreenFood;
  };

  btnExit = btn_factory.CreateButton(
      sf::Vector2f(window_.getSize().y - 30.f, 30.f), "Exit");
  btnExit->OnReleasedLeft = []() { window_.close(); };

  resource_manager.LoadResources(
      Resource::Type::kWood,
      tilemap_ptr_->GetCollectibles(TileMap::Tile::kTree), ChopEvent);

  resource_manager.LoadResources(
      Resource::Type::kFood,
      tilemap_ptr_->GetCollectibles(TileMap::Tile::kFood), ChopEvent);

  resource_manager.LoadResources(
      Resource::Type::kStone,
      tilemap_ptr_->GetCollectibles(TileMap::Tile::kRock), ChopEvent);
}

void SpawnInitialNpcs(int total) {
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
  std::uniform_int_distribution<std::ptrdiff_t> tile_dist(
      0, std::ssize(walkables) - 1);

  constexpr std::array<api::ai::NpcType, 3> kTypes = {
      api::ai::NpcType::kBlueWood,
      api::ai::NpcType::kRedRock,
      api::ai::NpcType::kGreenFood,
  };

  const int base = total / 3;
  const int remainder = total % 3;
  for (std::ptrdiff_t t = 0; t < std::ssize(kTypes); ++t) {
    const int count = base + (t < remainder ? 1 : 0);
    for (int i = 0; i < count; ++i) {
      const auto& position = walkables[tile_dist(gen)];
      npc_manager_.Add(kTypes[t], position, tilemap_ptr_.get(),
                       resource_manager);
    }
  }
}
}  // namespace

void Loop(const LaunchOptions& options) {
  Setup();
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

      tilemap_ptr_->HandleEvent(event, buttonsWasClicked);
    }

    // GamePlay, physic frame
    npc_manager_.Update(dt);

    PROFILE_PLOT("Frame dt (ms)", dt * 1000.0f);
    PROFILE_PLOT("NPC count", static_cast<int64_t>(npc_manager_.Count()));

    // Graphic frame
    window_.clear();

    tilemap_ptr_->Draw(window_);
    npc_manager_.Draw(window_);

    btnBlue->Draw(window_);
    btnRed->Draw(window_);
    btnGreen->Draw(window_);
    btnExit->Draw(window_);

    window_.display();
    PROFILE_FRAME();
  }
}
}  // namespace game
