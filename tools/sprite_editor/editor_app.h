#ifndef TOOLS_SPRITE_EDITOR_EDITOR_APP_H_
#define TOOLS_SPRITE_EDITOR_EDITOR_APP_H_

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "grid_slicer.h"
#include "sheet_document.h"

namespace sf {
class Event;
}

namespace sprite_editor {

// ImGui-SFML application: browse the sprites folder, open a spritesheet, slice
// it (grid + manual rectangles) with names and origins, and save the sidecar
// JSON plus a generated header.
class EditorApp {
 public:
  EditorApp();
  EditorApp(const EditorApp&) = delete;
  EditorApp& operator=(const EditorApp&) = delete;
  ~EditorApp();

  void Run();

 private:
  enum class Mode { kSelect, kDraw };

  void ScanAssets();
  void OpenSheet(const std::string& filename);
  void Save();

  void HandleEvent(const sf::Event& event);
  void PickSpriteAt(sf::Vector2f world);
  void FinishDrawnRect();

  void DrawCanvas();
  void DrawUi();
  void DrawSheetsPanel();
  void DrawToolsPanel();
  void DrawSpritesPanel();

  void FitView();
  void SyncNameBuffer();
  [[nodiscard]] sf::Vector2f MouseToWorld(sf::Vector2i pixel) const;
  [[nodiscard]] std::filesystem::path SidecarPath() const;

  sf::RenderWindow window_;
  sf::View canvas_view_;

  std::filesystem::path assets_dir_;
  std::filesystem::path generated_dir_;
  std::vector<std::string> png_files_;

  bool has_sheet_ = false;
  std::string sheet_filename_;
  sf::Texture sheet_texture_;
  std::optional<sf::Sprite> sheet_sprite_;
  SheetDocument document_;
  std::string status_message_;

  int selected_ = -1;
  Mode mode_ = Mode::kSelect;

  GridParams grid_;
  char name_prefix_[64] = "sprite_";
  char name_buf_[128] = "";

  // Canvas view: world units shown = window size / zoom_.
  float zoom_ = 1.f;
  sf::Vector2f canvas_center_{};

  // Interaction state.
  bool panning_ = false;
  sf::Vector2i pan_last_pixel_{};
  bool drawing_ = false;
  sf::Vector2f draw_start_world_{};
  sf::Vector2f draw_current_world_{};
};

}  // namespace sprite_editor

#endif  // TOOLS_SPRITE_EDITOR_EDITOR_APP_H_
