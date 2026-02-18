#include "ai/npc.h"

#include <random>
#include <utility>

#include "utils/log.h"

#include "motion/a_star.h"

#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

using namespace api::motion;

namespace api::ai{

    void Npc::Setup(std::string_view name, std::string_view filename,
                    const TileMap* tilemap, const sf::Vector2f& cantina_position, std::vector<Resource> ressources) {
#ifdef TRACY_ENABLE
        ZoneScoped;
#endif

        name_ = std::string(name);

        // TODO : find lighter concat for string view
        if(!texture_.loadFromFile("_assets/sprites/" + std::string(filename))) {
            core::LogError("Error loading texture {}", filename);
            if (!texture_.loadFromFile("_assets/sprites/empty.png")) {
                core::LogError("Error loading texture empty.png");
            }
        }

        core::LogDebug("Setup {} -- -- -- -- -- -- -- -- -- -- -- -- -- ", name_);

        bt_tree_->SetupBehaviourTree(motor_.get(), path_.get(), tilemap, cantina_position, std::move(ressources));

        motor_->SetPosition({0, 0});
        motor_->SetSpeed(kMovingSpeed);

    }

    void Npc::Update(const float dt){
#ifdef TRACY_ENABLE
        ZoneScoped;
#endif

        // // Updating
        // std::cout << "\n";
        // std::cout << " -- -- -- -- -- -- -- -- -- -- -- -- -- \n";
        // std::cout << "Update " << name_ << "\n";
        // std::cout << " -- -- -- -- -- -- -- -- -- -- -- -- -- \n";

        // -------------------
        if (path_->IsValid()){
            motor_->Update(dt);
            if (!path_->IsDone() && motor_->RemainingDistance() <= 0.001f) {
                motor_->SetDestination(path_->GetNextPoint());
            }
        }

        bt_tree_->Update(dt);

    }

    void Npc::Draw(sf::RenderWindow &window){
#ifdef TRACY_ENABLE
        ZoneScoped;
#endif
        sf::Sprite sprite(texture_);
        sprite.setPosition(motor_->GetPosition());
        window.draw(sprite);
    }

    // void Npc::SetPath(const Path& path){
    //     path_ = path;
    //     motor_.SetDestination(path_.StartPoint());
    // }

}
