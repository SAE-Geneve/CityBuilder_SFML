#include "ai/npc.h"

#include <iostream>
#include <random>

#include "motion/AStar.h"

using namespace api::motion;

namespace api::ai{

    void Npc::Setup(std::string_view name, std::string_view filename,
                    TileMap* tilemap) {

        name_ = std::string(name);

        // TODO : find lighter concat for string view
        if(!texture_.loadFromFile("_assets/sprites/" + std::string(filename))) {
            std::cout << "Error loading texture " << filename << std::endl;
            if (!texture_.loadFromFile("_assets/sprites/empty.png")) {
                std::cout << "Error loading texture empty.png" << std::endl;
            }
        }

        std::cout << "Setup " << name_ << " -- -- -- -- -- -- -- -- -- -- -- -- -- " << std::endl;

        bt_tree_->SetupBehaviourTree(motor_.get(), path_.get(), tilemap);

        motor_->SetPosition({0, 0});
        motor_->SetSpeed(kMovingSpeed);

    }

    void Npc::Update(const float dt){

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
        sf::Sprite sprite(texture_);
        sprite.setPosition(motor_->GetPosition());
        window.draw(sprite);
    }

    // void Npc::SetPath(const Path& path){
    //     path_ = path;
    //     motor_.SetDestination(path_.StartPoint());
    // }

}
