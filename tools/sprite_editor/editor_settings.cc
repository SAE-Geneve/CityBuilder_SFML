#include "editor_settings.h"

#include <format>
#include <fstream>

#include <nlohmann/json.hpp>

namespace sprite_editor {

std::filesystem::path SettingsPath() { return "editor_settings.json"; }

EditorSettings LoadSettings() {
  EditorSettings settings;

  std::ifstream stream(SettingsPath());
  if (!stream) {
    return settings;
  }

  nlohmann::json root;
  try {
    stream >> root;
  } catch (const nlohmann::json::exception&) {
    return settings;
  }

  settings.last_assets_dir = root.value("last_assets_dir", std::string{});
  return settings;
}

std::expected<void, std::string> SaveSettings(const EditorSettings& settings) {
  nlohmann::json root;
  root["last_assets_dir"] = settings.last_assets_dir.string();

  const auto path = SettingsPath();
  std::ofstream stream(path);
  if (!stream) {
    return std::unexpected(std::format("cannot write {}", path.string()));
  }
  stream << root.dump(2) << '\n';
  return {};
}

}  // namespace sprite_editor
