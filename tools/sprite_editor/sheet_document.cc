#include "sheet_document.h"

#include <format>
#include <fstream>

#include <nlohmann/json.hpp>

namespace sprite_editor {

std::expected<SheetDocument, std::string> LoadDocument(
    const std::filesystem::path& json_path) {
  std::ifstream stream(json_path);
  if (!stream) {
    return std::unexpected(
        std::format("cannot open {}", json_path.string()));
  }

  nlohmann::json root;
  try {
    stream >> root;
  } catch (const nlohmann::json::exception& e) {
    return std::unexpected(std::format("invalid JSON in {}: {}",
                                       json_path.string(), e.what()));
  }

  SheetDocument document;
  document.texture = root.value("texture", std::string{});

  if (const auto it = root.find("sprites"); it != root.end()) {
    for (const auto& node : *it) {
      EditorSprite sprite;
      sprite.name = node.value("name", std::string{});
      sprite.x = node.value("x", 0);
      sprite.y = node.value("y", 0);
      sprite.w = node.value("w", 0);
      sprite.h = node.value("h", 0);
      sprite.origin_x = node.value("origin_x", 0.f);
      sprite.origin_y = node.value("origin_y", 0.f);
      document.sprites.push_back(std::move(sprite));
    }
  }

  return document;
}

std::expected<void, std::string> SaveDocument(
    const SheetDocument& document, const std::filesystem::path& json_path) {
  nlohmann::json root;
  root["texture"] = document.texture;

  auto& sprites = root["sprites"] = nlohmann::json::array();
  for (const auto& sprite : document.sprites) {
    sprites.push_back({{"name", sprite.name},
                       {"x", sprite.x},
                       {"y", sprite.y},
                       {"w", sprite.w},
                       {"h", sprite.h},
                       {"origin_x", sprite.origin_x},
                       {"origin_y", sprite.origin_y}});
  }

  std::ofstream stream(json_path);
  if (!stream) {
    return std::unexpected(
        std::format("cannot write {}", json_path.string()));
  }
  stream << root.dump(2) << '\n';
  return {};
}

}  // namespace sprite_editor
