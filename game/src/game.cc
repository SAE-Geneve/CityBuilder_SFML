#include "game.h"

#include "ressource_manager.h"
#include "SFML/Graphics.hpp"
#include "ai/npc_manager.h"
#include "graphics/tilemap.h"
#include "ui/button.h"
#include "ui/button_factory.h"
#include "ui/clickable.h"

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

        RessourceManager ressource_manager_;

        void ChopEvent(int index, float quantity) {
            std::cout << "chop event : " << index << "," << quantity << "\n";
            if (quantity <= 0){
            	tilemap_ptr_->SetTile(index, TileMap::Tile::kBg);
            }
        }
		
        void Setup(){
            // Create the main window
            window_.create(sf::VideoMode({1280, 1080}), "SFML window");

            tilemap_ptr_->Setup();
            tilemap_ptr_->OnReleasedLeft = []() {
                std::cout << "Clicked tilemap" << "\n";
                npc_manager_.Add(npc_adding_type,
                                 TileMap::TilePos(sf::Mouse::getPosition(window_)),
                                 tilemap_ptr_.get(),
                                 ressource_manager_);
                npc_adding_type = api::ai::NpcType::kNone;
            };

            btnBlue = btn_factory.CreateButton(sf::Vector2f(100.f, window_.getSize().y - 100.f), "Blue");
            btnBlue->OnReleasedLeft = []() { npc_adding_type = api::ai::NpcType::kBlueWood; };

            btnRed = btn_factory.CreateButton(sf::Vector2f(200.f, window_.getSize().y - 100.f), "Red");
            btnRed->OnReleasedLeft = []() { npc_adding_type = api::ai::NpcType::kRedRock; };

            btnGreen = btn_factory.CreateButton(sf::Vector2f(300.f, window_.getSize().y - 100.f), "Green");
            btnGreen->OnReleasedLeft = []() { npc_adding_type = api::ai::NpcType::kGreenFood; };

            btnExit = btn_factory.CreateButton(sf::Vector2f(window_.getSize().y - 30.f, 30.f), "Exit");
            btnExit->OnReleasedLeft = []() { window_.close(); };

            ressource_manager_.LoadRessources(
				Ressource::Type::kWood,
				tilemap_ptr_->GetCollectibles(TileMap::Tile::kTree), ChopEvent);

			ressource_manager_.LoadRessources(
				Ressource::Type::kFood,
				tilemap_ptr_->GetCollectibles(TileMap::Tile::kFood), ChopEvent);

			ressource_manager_.LoadRessources(
				Ressource::Type::kStone,
				tilemap_ptr_->GetCollectibles(TileMap::Tile::kRock), ChopEvent);

        }
    } // namespace

    void Loop(){
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
                btnExit->HandleEvent(event, buttonsWasClicked);

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
            btnExit->Draw(window_);

            window_.display();
        }
    }
} // namespace game
