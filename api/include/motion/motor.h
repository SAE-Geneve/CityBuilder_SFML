//
// Created by sebas on 11/06/2025.
//

#ifndef MOTOR_H
#define MOTOR_H

#include <SFML/Graphics/Transform.hpp>

namespace api::motion {

    class Motor {
        sf::Vector2f position_;
        sf::Vector2f destination_;
        float speed_;

    public:
        // choix 2
        void Update(float dt);
        void MoveTo(sf::Vector2f destination);

        // getter / setter
        void SetSpeed(float speed){speed_ = speed;}
        void SetPosition(sf::Vector2f position){position_ = position;}
        [[nodiscard]] const sf::Vector2f& GetPosition() const{return position_;}

    };

}

#endif //MOTOR_H
