
#include "ai/npc.h"

#include <iostream>

Status Npc::Move(){
    std::cout << "I'm moving 2" << std::endl;
    return Status::kRunning;
}

void Npc::Setup(){
    textures.Load(files);

    root_ = std::make_unique<Action>([] () {
        std::cout << "I'm moving" << std::endl;
        return Status::kSuccess;
    });

    //root_ = std::make_unique<Action>(std::bind(&Npc::Move, this));

}

void Npc::Update(){
    root_->Tick();
}

void Npc::Draw(sf::RenderWindow &window){
    sf::Sprite sprite(textures.Get(Animation::KBlue));
    window.draw(sprite);
}



