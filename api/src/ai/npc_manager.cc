#include "ai/npc_manager.h"

#include <cassert>
#include <string_view>

#include "profiling/profiling.h"
#include "utils/log.h"

namespace api::ai {

namespace {

std::string_view SpriteFor(NpcType type) {
  switch (type) {
    case NpcType::kBlueWood:
      return "scifiUnit_06.png";
    case NpcType::kRedRock:
      return "scifiUnit_18.png";
    case NpcType::kGreenFood:
      return "scifiUnit_31.png";
    default:
      core::LogError("Error: Npc type not found");
      throw std::runtime_error("Error: Npc type not found");
  }
}

}  // namespace

NpcHandle NpcManager::Add(NpcType type, sf::Vector2f start_position,
                          TileMap* tilemap,
                          resource::ResourceManager& resource_manager) {
  PROFILE_ZONE();

  NpcHandle handle;
  if (!free_slots_.empty()) {
    handle = free_slots_.back();
    free_slots_.pop_back();
  } else {
    if (chunks_.empty() ||
        chunks_.back()->occupied[kChunkSize - 1]) {
      chunks_.emplace_back(std::make_unique<Chunk>());
      handle = {std::ssize(chunks_) - 1, 0};
    } else {
      auto& occ = chunks_.back()->occupied;
      std::ptrdiff_t slot = 0;
      while (slot < kChunkSize && occ[slot]) ++slot;
      handle = {std::ssize(chunks_) - 1, slot};
    }
  }

  Chunk& chunk = *chunks_[handle.chunk];
  chunk.occupied[handle.slot] = true;
  ++alive_count_;

  Npc& npc = chunk.npcs[handle.slot];
  npc.Setup(type, SpriteFor(type), tilemap, start_position, resource_manager);
  npc.set_position(start_position);
  return handle;
}

void NpcManager::Erase(NpcHandle handle) {
  PROFILE_ZONE();
  assert(handle.chunk >= 0 && handle.chunk < std::ssize(chunks_));
  assert(handle.slot >= 0 && handle.slot < kChunkSize);
  Chunk& chunk = *chunks_[handle.chunk];
  assert(chunk.occupied[handle.slot]);
  chunk.occupied[handle.slot] = false;
  free_slots_.push_back(handle);
  --alive_count_;
}

void NpcManager::Update(float dt) {
  PROFILE_ZONE();
  for (auto& chunk_ptr : chunks_) {
    Chunk& chunk = *chunk_ptr;
    for (std::ptrdiff_t i = 0; i < kChunkSize; ++i) {
      if (chunk.occupied[i]) chunk.npcs[i].Update(dt);
    }
  }
}

void NpcManager::Draw(sf::RenderWindow& window) {
  PROFILE_ZONE();
  for (auto& chunk_ptr : chunks_) {
    Chunk& chunk = *chunk_ptr;
    for (std::ptrdiff_t i = 0; i < kChunkSize; ++i) {
      if (chunk.occupied[i]) chunk.npcs[i].Draw(window);
    }
  }
}

}  // namespace api::ai
