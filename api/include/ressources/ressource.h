//
// Created by sebas on 26/06/2025.
//

#ifndef RESSOURCE_H
#define RESSOURCE_H

#include <SFML/System/Vector2.hpp>
#include <algorithm>

class Ressource {

public:
    enum class Type {
        kNone, kWood, kStone, kFood
    };

private:
    int tile_index_ = 0;
    float quantity_ = 0;
    Type type_ = Type::kNone;
    double cut_time_ = 0;

public:
	[[nodiscard]] Type GetType() const;
	[[nodiscard]] int GetTileIndex() const;
	[[nodiscard]] float GetQty() const;

    void SetType(Type type);
    void SetIndex(int index);
    void SetQuantity(float quantity);

    void Exploit(float);

    std::function<void(int, float)> OnChopRessource_ = nullptr;

};

inline void Ressource::SetType(Type type){
    type_ = type;
}
inline void Ressource::SetIndex(int index){
    tile_index_ = index;
}
inline void Ressource::SetQuantity(float quantity){
    quantity_ = quantity;
}

inline Ressource::Type Ressource::GetType() const{
	return type_;
}
inline int Ressource::GetTileIndex() const {
	return tile_index_;
}
inline float Ressource::GetQty() const {
	return quantity_;
}


inline void Ressource::Exploit(float rate) {
	quantity_ -= rate;
	quantity_ = std::max<float>(quantity_, 0);

    if (OnChopRessource_) {
		OnChopRessource_(tile_index_, quantity_);
    }
}
#endif //RESSOURCE_H
