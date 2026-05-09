#ifndef API_AI_NPC_H_
#define API_AI_NPC_H_

// TODO(google-style): narrow this umbrella include to the specific SFML
// headers actually used by this header.
#include <SFML/Graphics.hpp>

#include "graphics/tilemap.h"
#include "motion/motor.h"
#include "motion/path.h"
#include "resources/resource_manager.h"

namespace api::ai {

class NpcBehaviourTree;
enum class NpcType : char {
  kNone = 'n',
  kBlueWood = 'w',
  kRedRock = 'r',
  kGreenFood = 'f'
};

class Npc {
  // FIXME load the texture from an AssetManager?
  std::unique_ptr<sf::Texture> texture_ = std::make_unique<sf::Texture>();
  std::optional<sf::Sprite> sprite_;

  // FIXME each npc has its own bt_tree that allocates which is the same...
  // What about 4 bt_trees (one for each role) shared between all NPC
  std::unique_ptr<NpcBehaviourTree> bt_tree_;

  // Movement
  static constexpr float kMovingSpeed = 200.0f;
  motion::Motor motor_;
  motion::Path path_;

  NpcType type_ = NpcType::kNone;

 public:
  void Setup(NpcType type, std::string_view filename, const TileMap* tilemap,
             const sf::Vector2f& cantina_position,
             resource::ResourceManager& resources);
  void Update(float dt);
  void Draw(sf::RenderWindow& window);
  void set_position(const sf::Vector2f& position) {
    motor_.set_position(position);
  }
  [[nodiscard]] NpcType type() const { return type_; }
};

constexpr resource::Resource::Type GetResourceType(NpcType type) {
  switch (type) {
    case NpcType::kBlueWood:
      return resource::Resource::Type::kWood;
    case NpcType::kGreenFood:
      return resource::Resource::Type::kFood;
    case NpcType::kRedRock:
      return resource::Resource::Type::kStone;
    default:
      break;
  }
  return resource::Resource::Type::kNone;
}
}  // namespace api::ai

#endif  // API_AI_NPC_H_
