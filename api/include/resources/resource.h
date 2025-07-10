//
// Created by sebas on 26/06/2025.
//

#ifndef RESOURCE_H
#define RESOURCE_H

#include <algorithm>

class Resource {

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

    std::function<void(int, float)> OnChopResource_ = nullptr;

};

inline void Resource::SetType(const Type type){
    type_ = type;
}
inline void Resource::SetIndex(const int index){
    tile_index_ = index;
}
inline void Resource::SetQuantity(const float quantity){
    quantity_ = quantity;
}

inline Resource::Type Resource::GetType() const{
    return type_;
}
inline int Resource::GetTileIndex() const {
    return tile_index_;
}
inline float Resource::GetQty() const {
    return quantity_;
}


inline void Resource::Exploit(const float rate) {
    quantity_ -= rate;
    quantity_ = std::max<float>(quantity_, 0);

    if (OnChopResource_) {
        OnChopResource_(tile_index_, quantity_);
    }
}
#endif
