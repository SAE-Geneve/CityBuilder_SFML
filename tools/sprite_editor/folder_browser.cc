#include "folder_browser.h"

#include <algorithm>
#include <cstring>
#include <format>
#include <system_error>

#include <imgui.h>

namespace sprite_editor {

namespace {

constexpr const char* kPopupName = "Open folder";

// Copies a path into the editable buffer, always null-terminated.
void SetBuffer(char* buf, std::size_t size, const std::string& value) {
  const std::size_t count = std::min(value.size(), size - 1);
  std::memcpy(buf, value.data(), count);
  buf[count] = '\0';
}

}  // namespace

void FolderBrowser::Open(const std::filesystem::path& start_dir) {
  std::error_code ec;
  std::filesystem::path dir = start_dir;
  if (dir.empty() || !std::filesystem::is_directory(dir, ec)) {
    dir = std::filesystem::current_path(ec);
  }
  NavigateTo(dir);
  open_requested_ = true;
}

void FolderBrowser::NavigateTo(const std::filesystem::path& dir) {
  std::error_code ec;
  const std::filesystem::path canonical = std::filesystem::weakly_canonical(dir, ec);
  current_dir_ = ec ? dir : canonical;
  SetBuffer(path_buf_, sizeof(path_buf_), current_dir_.string());
  RefreshEntries();
}

void FolderBrowser::RefreshEntries() {
  subdirs_.clear();
  error_.clear();

  std::error_code ec;
  std::filesystem::directory_iterator it(current_dir_, ec);
  if (ec) {
    error_ = std::format("cannot read {}: {}", current_dir_.string(),
                         ec.message());
    return;
  }

  for (const auto& entry : it) {
    std::error_code dir_ec;
    if (entry.is_directory(dir_ec)) {
      subdirs_.push_back(entry.path());
    }
  }
  std::ranges::sort(subdirs_);
}

std::optional<std::filesystem::path> FolderBrowser::Draw() {
  if (open_requested_) {
    ImGui::OpenPopup(kPopupName);
    open_requested_ = false;
  }

  // Center the modal on first appearance.
  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2(560.f, 420.f), ImGuiCond_Appearing);

  std::optional<std::filesystem::path> result;

  if (ImGui::BeginPopupModal(kPopupName, nullptr,
                             ImGuiWindowFlags_NoSavedSettings)) {
    // Editable path: type/paste a folder (also lets the user hop drives on
    // Windows, which "[..]" alone cannot reach).
    if (ImGui::InputText("##path", path_buf_, sizeof(path_buf_),
                         ImGuiInputTextFlags_EnterReturnsTrue)) {
      std::error_code ec;
      const std::filesystem::path typed(static_cast<const char*>(path_buf_));
      if (std::filesystem::is_directory(typed, ec)) {
        NavigateTo(typed);
      } else {
        error_ = std::format("not a folder: {}", typed.string());
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("Go")) {
      std::error_code ec;
      const std::filesystem::path typed(static_cast<const char*>(path_buf_));
      if (std::filesystem::is_directory(typed, ec)) {
        NavigateTo(typed);
      } else {
        error_ = std::format("not a folder: {}", typed.string());
      }
    }

    ImGui::Separator();

    ImGui::BeginChild("entries", ImVec2(0.f, 280.f), true);
    if (current_dir_.has_parent_path() &&
        current_dir_.parent_path() != current_dir_) {
      if (ImGui::Selectable("[..]")) {
        NavigateTo(current_dir_.parent_path());
      }
    }
    for (const auto& dir : subdirs_) {
      const std::string label =
          std::format("[{}]", dir.filename().string());
      if (ImGui::Selectable(label.c_str())) {
        NavigateTo(dir);
        break;
      }
    }
    ImGui::EndChild();

    if (!error_.empty()) {
      ImGui::TextWrapped("%s", error_.c_str());
    }

    ImGui::Separator();
    if (ImGui::Button("Select this folder")) {
      result = current_dir_;
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }

  return result;
}

}  // namespace sprite_editor
