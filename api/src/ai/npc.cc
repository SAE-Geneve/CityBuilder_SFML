#include "ai/npc.h"

#include <format>
#include <print>

#include "ai/bt_action.h"
#include "ai/bt_node_factory.h"
#include "ai/bt_selector.h"
#include "ai/bt_sequence.h"
#include "rng/rng.h"


namespace api::ai {
    using core::ai::behaviour_tree::Status;

    void Npc::Setup(const sf::Texture* texture, const sf::Vector2f world_size,
                    const sf::Vector2f start_position){
        world_size_ = world_size;

        if (texture != nullptr) {
            sprite_ = sf::Sprite(*texture);
        }

        motor_.set_position(start_position);
        motor_.set_destination(start_position); // stay put until the first pick
        motor_.set_speed(kSpeed);

        using namespace core::ai::behaviour_tree::node_factory;

        auto testSequence = MakeSequence();
        testSequence->AddChild(MakeAction([] {return Status::kSuccess;}));
        testSequence->AddChild(MakeAction([] {return Status::kFailure;}));
        testSequence->AddChild(MakeAction([this] { return Locked();}));

        auto wanderSequence = MakeSequence();
        wanderSequence->AddChild(MakeAction([this] { return MoveToDestination(); }));
        wanderSequence->AddChild(MakeAction([this] { return PickRandomDestination(); }));

        auto basicSelector = MakeSelector();
        basicSelector->AddChild(std::move(testSequence));
        basicSelector->AddChild(std::move(wanderSequence));

        bt_root_ = std::move(basicSelector);

        // Rough wander behavior :
        // Sequence (pick a random destination, then move to it)
        // PickRandomDestination always succeeds, MoveToDestination stays kRunning
        // until the motor reaches the target. When the sequence completes, it resets

    }

    void Npc::Update(const float dt){
        motor_.Update(dt);
        if (bt_root_) {
            bt_root_->Tick();
        }
    }

    void Npc::Draw(sf::RenderWindow &window){
        if (sprite_.has_value()) {
            sprite_->setPosition(motor_.position());
            window.draw(*sprite_);
        }
    }

    Status Npc::PickRandomDestination(){

        motor_.set_destination({core::rng::get_value(0.f,12.f),core::rng::get_value(0.f,12.f)});
        // get the path
        return Status::kSuccess;
    }

    Status Npc::MoveToDestination() const{

        // on parcourt case par case, waypoints

        return motor_.remaining_distance() <= 0.001f
                   ? Status::kSuccess
                   : Status::kRunning;
    }

    Status Npc::Locked(){
        std::println("locked");
        return Status::kRunning;
    }
} // namespace api::ai
