#include "game.h"

#include "SFML/Graphics.hpp"
#include "ai/npc_manager.h"
#include "graphics/tilemap.h"
#include "ui/button.h"
#include "ui/clickable.h"

namespace game {
namespace {

sf::Clock clock;
sf::RenderWindow window_;

auto tilemap_ptr_ = std::make_unique<TileMap>();
api::ai::NpcManager npc_manager_;

// UI Elements
std::unique_ptr<api::ui::Button> btnBlue;
std::unique_ptr<api::ui::Button> btnRed;
std::unique_ptr<api::ui::Button> btnGreen;

sf::Font font;

api::ai::NpcType npc_adding_type = api::ai::NpcType::kNone;

enum class UISprite { kEmpty, kBgBtn, kHoverBtn, kLength };
std::array<std::string_view, static_cast<size_t>(UISprite::kLength)> UIfiles_ =
    {"empty.png", "button_grey.png", "button_brown.png"};
AssetManager<sf::Texture, UISprite, "_assets/sprites"> UItextures_;

void Setup() {
  // Create the main window
  window_.create(sf::VideoMode({1280, 1080}), "SFML window");

  tilemap_ptr_->Setup();
  tilemap_ptr_->OnReleasedLeft = []() {

    std::cout << "Clicked tilemap" << "\n";
    npc_manager_.Add(npc_adding_type,
                     TileMap::TilePos(sf::Mouse::getPosition(window_)),
                     tilemap_ptr_.get());
    npc_adding_type = api::ai::NpcType::kNone;

  };

  UItextures_.Load(UIfiles_);
  font.openFromFile("_assets/fonts/ANTQUAB.TTF");

  btnBlue = std::make_unique<api::ui::Button>(
      sf::Vector2f(100.f, window_.getSize().y - 100.f), "Blue",
      UItextures_.Get(UISprite::kBgBtn), UItextures_.Get(UISprite::kHoverBtn),
      font);
  btnBlue->OnReleasedLeft = []() { npc_adding_type = api::ai::NpcType::kBlue; };

  btnRed = std::make_unique<api::ui::Button>(
      sf::Vector2f(200.f, window_.getSize().y - 100.f), "Red",
      UItextures_.Get(UISprite::kBgBtn), UItextures_.Get(UISprite::kHoverBtn),
      font);
  btnRed->OnReleasedLeft = []() { npc_adding_type = api::ai::NpcType::kRed; };

  btnGreen = std::make_unique<api::ui::Button>(
      sf::Vector2f(300.f, window_.getSize().y - 100.f), "Green",
      UItextures_.Get(UISprite::kBgBtn), UItextures_.Get(UISprite::kHoverBtn),
      font);
  btnGreen->OnReleasedLeft = []() {
    npc_adding_type = api::ai::NpcType::kGreen;
  };
}
}  // namespace

void Loop() {
  Setup();

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

      tilemap_ptr_->HandleEvent(event, buttonsWasClicked);

    }

    // GamePlay, physic frame
    npc_manager_.Update(dt);

    // Graphic frame
    window_.clear();

    tilemap_ptr_->Draw(window_);
    npc_manager_.Draw(window_);

    btnBlue->Draw(window_);
    btnRed->Draw(window_);
    btnGreen->Draw(window_);

    window_.display();
  }
}
}  // namespace game
