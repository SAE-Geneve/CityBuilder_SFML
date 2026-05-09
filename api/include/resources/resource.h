#ifndef API_RESOURCES_RESOURCE_H_
#define API_RESOURCES_RESOURCE_H_

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

  // FIXME this generates allocation
  void(*on_chop_resource_)(int, float) = nullptr;
};
}
#endif  // API_RESOURCES_RESOURCE_H_
