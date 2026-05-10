#ifndef API_AI_NPC_MANAGER_H_
#define API_AI_NPC_MANAGER_H_

#include <array>
#include <memory>
#include <vector>

#include "ai/npc.h"
#include "ai/npc_behaviour_tree.h"
#include "resources/resource_manager.h"

namespace api::ai {

struct NpcHandle {
  std::ptrdiff_t chunk = -1;
  std::ptrdiff_t slot = -1;
};

class NpcManager {
  static constexpr size_t kChunkSize = 64;

  struct Chunk {
    std::array<Npc, kChunkSize> npcs;
    std::array<bool, kChunkSize> occupied{};
  };

  std::vector<std::unique_ptr<Chunk>> chunks_;
  std::vector<NpcHandle> free_slots_;
  std::ptrdiff_t alive_count_ = 0;

 public:
  NpcHandle Add(NpcType type, sf::Vector2f start_position, TileMap* tilemap,
                resource::ResourceManager& resource_manager);
  void Erase(NpcHandle handle);
  void Update(float dt);
  void Draw(sf::RenderWindow& window);

  [[nodiscard]] std::ptrdiff_t Count() const noexcept { return alive_count_; }
};
}  // namespace api::ai

#endif  // API_AI_NPC_MANAGER_H_
