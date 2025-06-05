
#include "ai/npc.h"

#include <iostream>



Status Npc::Move(){
    std::cout << "I'm moving 2" << std::endl;
    return Status::kRunning;
}

void Npc::Setup(){
    textures.Load(files);

    auto selector = std::make_unique<Selector>();
    selector->AddChild(std::make_unique<Action>([this] () {
        if (hunger_ >= 100){
            std::cout << "I'm hungry, eating........" << std::endl;
            hunger_ = 0;
            return Status::kSuccess;
        }
        return Status::kFailure;
    }));
    selector->AddChild(std::make_unique<Action>([this] () {
        if (resourceAvailable_){
            std::cout << "Resource Available, working....." << std::endl;
            return Status::kSuccess;
        }
        return Status::kFailure;
    }));
    selector->AddChild(std::make_unique<Action>([this] () {
        std::cout << "I'm sleeping" << std::endl;
        return Status::kSuccess;
    }));


    root_ = std::move(selector);

    //root_ = std::make_unique<Action>(std::bind(&Npc::Move, this));

}

void Npc::Update(){

    // -------------------
    hunger_++;
    std::cout << "Hunger : " << hunger_ << std::endl;
    root_->Tick();
}

void Npc::Draw(sf::RenderWindow &window){
    sf::Sprite sprite(textures.Get(Animation::KBlue));
    window.draw(sprite);
}



