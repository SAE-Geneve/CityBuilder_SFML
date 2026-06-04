#ifndef TOOLS_SPRITE_EDITOR_FOLDER_BROWSER_H_
#define TOOLS_SPRITE_EDITOR_FOLDER_BROWSER_H_

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace sprite_editor {

// In-window folder picker rendered as an ImGui modal popup. Call Open() to
// request it, then call Draw() every frame; Draw() returns the chosen folder on
// the frame the user confirms (std::nullopt on every other frame).
class FolderBrowser {
 public:
  void Open(const std::filesystem::path& start_dir);

  [[nodiscard]] std::optional<std::filesystem::path> Draw();

 private:
  void NavigateTo(const std::filesystem::path& dir);
  void RefreshEntries();

  bool open_requested_ = false;
  std::filesystem::path current_dir_;
  std::vector<std::filesystem::path> subdirs_;  // sorted child directories
  std::string error_;
  char path_buf_[512] = "";  // editable path field (drive hops, paste)
};

}  // namespace sprite_editor

#endif  // TOOLS_SPRITE_EDITOR_FOLDER_BROWSER_H_
