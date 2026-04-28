#ifndef NPC_H
#define NPC_H

#include <SFML/Graphics.hpp>

#include "graphics/tilemap.h"
#include "motion/motor.h"
#include "motion/path.h"
#include "npc_behaviour_tree.h"
#include "resources/resource.h"

namespace api::ai {

enum class NpcType {
  kNone = 'n',
  kBlueWood = 'w',
  kRedRock = 'r',
  kGreenFood = 'f'
};

class Npc {
  std::unique_ptr<sf::Texture> texture_ = std::make_unique<sf::Texture>();
  std::optional<sf::Sprite> sprite_;

  /*
   * FIXME each npc has its own bt_tree that allocates which is the same...
   * What about 4 bt_trees (one for each role) shared between all NPC
   */
  std::unique_ptr<NpcBehaviourTree> bt_tree_ = std::make_unique<NpcBehaviourTree>();

  // Movement
  static constexpr float kMovingSpeed = 200.0f;
  std::unique_ptr<motion::Motor> motor_ = std::make_unique<motion::Motor>();
  std::unique_ptr<motion::Path> path_ = std::make_unique<motion::Path>();

  // Replace with type
  NpcType type_ = NpcType::kNone;

  // World informations
  // Tilemap
  // const TileMap *tileMap_;

 public:
  void Setup(NpcType type, std::string_view filename,
             const TileMap* tilemap, const sf::Vector2f& cantina_position,
             std::vector<Resource> ressources);
  void Update(float dt);
  void Draw(sf::RenderWindow& window);
  void SetPosition(const sf::Vector2f& position) {
    motor_->SetPosition(position);
  }

  // Motion
  // void SetPath(const motion::Path &path);
};
}  // namespace api::ai

#endif  // NPC_H
