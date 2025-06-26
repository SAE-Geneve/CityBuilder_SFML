//
// Created by sebas on 26/06/2025.
//

#ifndef RESSOURCE_H
#define RESSOURCE_H

#include <SFML/System/Vector2.hpp>

class Ressource {

public:
    enum class Type {
        kNone, kWood, kStone, kFood
    };

private:
    int tile_index_ = 0;
    int quantity_ = 0;
    Type type_ = Type::kNone;
    double cut_time_ = 0;

public:
    void SetType(Type type);
    void SetIndex(int index);
    void SetQuantity(int quantity);
};

inline void Ressource::SetType(Type type){
    type_ = type;
}
inline void Ressource::SetIndex(int index){
    tile_index_ = index;
}
inline void Ressource::SetQuantity(int quantity){
    quantity_ = quantity;
}

#endif //RESSOURCE_H
