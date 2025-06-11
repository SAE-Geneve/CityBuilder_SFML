#ifndef MOTOR_H
#define MOTOR_H

namespace api::motion {

    class Motor {
        sf::Vector2f position_;
        sf::Vector2f destination_;
        float speed_;

    public:
        void Update(float dt);  // each frame

        // getter / setter
        void SetSpeed(float speed){speed_ = speed;}
        void SetPosition(sf::Vector2f position){position_ = position;}
        void SetDestination(sf::Vector2f destination){destination_ = destination;}

        [[nodiscard]] const sf::Vector2f& GetPosition() const{return position_;}

    };

    inline void Motor::Update(const float dt){
       const sf::Vector2f distance = destination_ - position_;

        if (distance.length() < speed_ * dt) {
            position_ = destination_;
            return;
        }
        position_ += distance.normalized() * speed_ * dt;

    }


}

#endif //MOTOR_H
