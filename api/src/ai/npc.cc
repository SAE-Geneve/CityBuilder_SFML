#include "ai/npc.h"

#include <iostream>

#include "ai/bt_sequence.h"

Status Npc::Move(){
    // if destination not reachable, return failure
    if (!target_reachable_) {
        std::cout << "Not reachable" << target_reachable_ << std::endl;
        return Status::kFailure;
    } else{
        std::cout << "I'm moving (distance = " << target_distance_ << ")" << std::endl;
        if (target_distance_ >= 0.15f) {
            // still arriving, return running
            target_distance_ -= kMovingSpeed;
            return Status::kRunning;
        } else {
            // if destination reached, return success
            return Status::kSuccess;
        }
    }
}



Status Npc::Eat(){
    // No failure, until we have food storage system
    hunger_ -= kHungerRate;
    if (hunger_ > 0) {
        return Status::kRunning;
    }else {
        return Status::kSuccess;
    }
}

void Npc::Setup(){
    textures.Load(files);

    auto feedSequence = std::make_unique<Sequence>();
    feedSequence->AddChild(std::make_unique<Action>([this]() {
        if (hunger_ >= 100) {
            std::cout << "I'm hungry, wanna eat........" << std::endl;
            return Status::kSuccess;
        }else {
            std::cout << "I'm not hungry, thanks........" << std::endl;
            return Status::kFailure;
        }

    }));
    feedSequence->AddChild(std::make_unique<Action>(std::bind(&Npc::Move, this)));
    feedSequence->AddChild(std::make_unique<Action>(std::bind(&Npc::Eat, this)));



    auto selector = std::make_unique<Selector>();
    // Attach the sequence to the selector
    selector->AddChild(std::move(feedSequence));
    // Work sequence
    selector->AddChild(std::make_unique<Action>([this]() {
        hunger_ += kHungerRate * 5;
        if (resourceAvailable_) {
            std::cout << "Resource Available, working....." << std::endl;
            return Status::kSuccess;
        }
        return Status::kFailure;
    }));
    // Idle sequence
    selector->AddChild(std::make_unique<Action>([this]() {
        hunger_ += kHungerRate * 5;
        std::cout << "I'm sleeping" << std::endl;
        return Status::kSuccess;
    }));

    root_ = std::move(selector);

    //root_ = std::make_unique<Action>(std::bind(&Npc::Move, this));
}

void Npc::Update(){
    // -------------------
    std::cout << "Hunger : " << hunger_ << std::endl;
    root_->Tick();
}

void Npc::Draw(sf::RenderWindow &window){
    sf::Sprite sprite(textures.Get(Animation::KBlue));
    window.draw(sprite);
}
