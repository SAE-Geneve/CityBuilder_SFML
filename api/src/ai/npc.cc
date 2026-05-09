#include "ai/npc.h"

#include "ai/npc_behaviour_tree.h"
#include "profiling/profiling.h"
#include "resources/resource_manager.h"
#include "utils/log.h"

namespace api::ai {

void Npc::Setup(const NpcType type, std::string_view filename,
                const TileMap* tilemap, const sf::Vector2f& cantina_position,
                resource::ResourceManager& resources) {
  PROFILE_ZONE();

  type_ = type;

  bool texture_is_fine = false;

  texture_is_fine =
      texture_->loadFromFile(std::format("_assets/sprites/{}", filename));
  if (!texture_is_fine) {
    core::LogError("Error loading texture {}", filename);

    texture_is_fine = texture_->loadFromFile("_assets/sprites/empty.png");
    if (!texture_is_fine) {
      core::LogError("Error loading texture empty.png");
    }
  }

  if (texture_is_fine) {
    sprite_ = sf::Sprite(*texture_);
  }

  core::LogDebug("Setup {} -- -- -- -- -- -- -- -- -- -- -- -- -- ",
                 static_cast<char>(type_));
  bt_tree_ = std::make_unique<NpcBehaviourTree>(resources);
  bt_tree_->SetupBehaviourTree(&motor_, &path_, this, tilemap,
                               cantina_position);

  motor_.set_position({0, 0});
  motor_.set_speed(kMovingSpeed);
}

void Npc::Update(const float dt) {
  PROFILE_ZONE();

  if (path_.valid()) {
    motor_.Update(dt);
    if (!path_.IsDone() && motor_.remaining_distance() <= 0.001f) {
      motor_.set_destination(path_.next_point());
    }
  }

  bt_tree_->Update(dt);
}

void Npc::Draw(sf::RenderWindow& window) {
  PROFILE_ZONE();
  if (sprite_.has_value()) {
    sprite_.value().setPosition(motor_.position());
    window.draw(sprite_.value());
  }
}

}  // namespace api::ai
