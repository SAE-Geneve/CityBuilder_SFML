#ifndef TOOLS_SPRITE_EDITOR_SHEET_DOCUMENT_H_
#define TOOLS_SPRITE_EDITOR_SHEET_DOCUMENT_H_

#include <expected>
#include <filesystem>
#include <string>
#include <vector>

namespace sprite_editor {

// One sprite as edited in the tool. Mirrors api::graphics::SpriteRect plus a
// name; kept independent so the tool does not link the api library.
struct EditorSprite {
  std::string name;
  int x = 0;
  int y = 0;
  int w = 0;
  int h = 0;
  float origin_x = 0.f;
  float origin_y = 0.f;
};

// In-memory model of a `<sheet>.sprites.json` sidecar file.
struct SheetDocument {
  std::string texture;  // texture filename, relative to the sprites folder
  std::vector<EditorSprite> sprites;
};

// Reads the sidecar JSON. Returns the parsed document, or an error string for
// unexpected failures (missing file is handled by the caller before calling).
std::expected<SheetDocument, std::string> LoadDocument(
    const std::filesystem::path& json_path);

// Writes the sidecar JSON (pretty-printed). Returns an error string on failure.
std::expected<void, std::string> SaveDocument(
    const SheetDocument& document, const std::filesystem::path& json_path);

}  // namespace sprite_editor

#endif  // TOOLS_SPRITE_EDITOR_SHEET_DOCUMENT_H_
