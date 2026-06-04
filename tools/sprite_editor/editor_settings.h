#ifndef TOOLS_SPRITE_EDITOR_EDITOR_SETTINGS_H_
#define TOOLS_SPRITE_EDITOR_EDITOR_SETTINGS_H_

#include <expected>
#include <filesystem>
#include <string>

namespace sprite_editor {

// Persisted editor preferences, stored as "editor_settings.json" next to the
// working directory so the tool reopens on the folder it was last used with.
struct EditorSettings {
  std::filesystem::path last_assets_dir;
};

// Location of the settings file (relative to the current working directory).
[[nodiscard]] std::filesystem::path SettingsPath();

// Reads the settings file. A missing or corrupt file is not an error: defaults
// are returned instead.
[[nodiscard]] EditorSettings LoadSettings();

// Writes the settings file (pretty-printed). Returns an error string on failure.
std::expected<void, std::string> SaveSettings(const EditorSettings& settings);

}  // namespace sprite_editor

#endif  // TOOLS_SPRITE_EDITOR_EDITOR_SETTINGS_H_
