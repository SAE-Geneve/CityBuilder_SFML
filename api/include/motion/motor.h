#ifndef MOTOR_H
#define MOTOR_H

#include <SFML/System/Vector2.hpp>
#include <iostream>

namespace api::motion {

    class Motor {
        sf::Vector2f position_;
        sf::Vector2f destination_;
        float speed_ = 0;

        float remainingDistance_ = 0;

    public:
        void Update(float dt);  // each frame
        [[nodiscard]] float RemainingDistance() const;

        // getter / setter
        void SetSpeed(float speed){speed_ = speed;}
        void SetPosition(sf::Vector2f position){position_ = position;}
        void SetDestination(const sf::Vector2f destination){
            //std::cout << "new destination : " << destination.x << ":" << destination.y << "\n";
            destination_ = destination;
        }

        [[nodiscard]] const sf::Vector2f& GetPosition() const{return position_;}


    };

    inline void Motor::Update(const float dt){
       const sf::Vector2f distance = destination_ - position_;
        remainingDistance_ = distance.length();

        if (remainingDistance_ < speed_ * dt) {
            position_ = destination_;
            return;
        }
        position_ += distance.normalized() * speed_ * dt;

    }

    inline float Motor::RemainingDistance() const{
        return remainingDistance_;
    }



}

#endif //MOTOR_H
