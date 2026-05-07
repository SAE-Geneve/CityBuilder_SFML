//
// Created by sebas on 26/06/2025.
//

#ifndef RESOURCE_H
#define RESOURCE_H

#include <algorithm>

namespace api::resource {
class Resource {
public:
  enum class Type { kNone, kWood, kStone, kFood };

private:
  int tile_index_ = 0;
  float quantity_ = 0;
  Type type_ = Type::kNone;

public:
  [[nodiscard]] Type type() const {return type_; }
 [[nodiscard]] int tile_index() const {return tile_index_; }
 [[nodiscard]] float quantity() const {return quantity_; }

  void set_type(Type type){type_ = type;}
  void set_index(int index){tile_index_ = index;}
  void set_quantity(float quantity){quantity_ = quantity;}

  void Exploit(float);

  //FIXME this generates allocation
  //std::function<void(int, float)> OnChopResource_ = nullptr;
  void(*on_chop_resource_)(int, float) = nullptr;
};
}
#endif
