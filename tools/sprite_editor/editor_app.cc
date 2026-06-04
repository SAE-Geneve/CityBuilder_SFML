#include "editor_app.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <format>
#include <fstream>

#include <imgui-SFML.h>
#include <imgui.h>

#include "editor_settings.h"
#include "grid_slicer.h"
#include "header_generator.h"
#include "utils/log.h"

namespace sprite_editor {

EditorApp::EditorApp()
    : window_(sf::VideoMode({1280u, 800u}), "Sprite Editor"),
      canvas_view_(window_.getDefaultView()),
      assets_dir_("_assets/sprites") {
  window_.setFramerateLimit(60);
  // Init(loadDefaultFont = true) loads the default font and builds the atlas via
  // the classic static-texture path. The project pins imgui to 1.91 / imgui-sfml
  // 3.0#2 (see vcpkg.json overrides) precisely to keep this path, because the
  // 1.92 dynamic-texture backend in imgui-sfml 3.0#3 scrambles every glyph.
  static_cast<void>(ImGui::SFML::Init(window_));

  // Reopen on the last-used folder when it still exists; otherwise keep the
  // default sprites directory.
  const EditorSettings settings = LoadSettings();
  std::error_code ec;
  if (!settings.last_assets_dir.empty() &&
      std::filesystem::is_directory(settings.last_assets_dir, ec)) {
    assets_dir_ = settings.last_assets_dir;
  }

  ScanAssets();
}

EditorApp::~EditorApp() { ImGui::SFML::Shutdown(); }

void EditorApp::Run() {
  sf::Clock delta_clock;
  while (window_.isOpen()) {
    while (const std::optional event = window_.pollEvent()) {
      ImGui::SFML::ProcessEvent(window_, *event);
      HandleEvent(*event);
    }

    ImGui::SFML::Update(window_, delta_clock.restart());
    DrawUi();
    if (auto picked = folder_browser_.Draw()) {
      ChangeAssetsDir(*picked);
    }

    window_.clear(sf::Color(40, 40, 40));
    if (has_sheet_) {
      DrawCanvas();
      window_.setView(ScreenView());
    }
    ImGui::SFML::Render(window_);
    window_.display();
  }
}

void EditorApp::ScanAssets() {
  png_files_.clear();
  std::error_code ec;
  if (!std::filesystem::exists(assets_dir_, ec)) {
    status_message_ =
        std::format("assets dir not found: {}", assets_dir_.string());
    return;
  }
  for (const auto& entry : std::filesystem::directory_iterator(assets_dir_, ec)) {
    if (!entry.is_regular_file()) {
      continue;
    }
    const std::string ext = entry.path().extension().string();
    if (ext == ".png" || ext == ".PNG") {
      png_files_.push_back(entry.path().filename().string());
    }
  }
  std::ranges::sort(png_files_);
}

void EditorApp::ChangeAssetsDir(const std::filesystem::path& dir) {
  assets_dir_ = dir;

  // Drop any sheet from the previous folder so a stale document is not shown.
  has_sheet_ = false;
  sheet_filename_.clear();
  sheet_sprite_.reset();
  document_ = SheetDocument{};
  selected_ = -1;

  ScanAssets();

  if (auto result = SaveSettings({.last_assets_dir = assets_dir_}); !result) {
    status_message_ = result.error();
    return;
  }
  status_message_ = std::format("assets folder: {}", assets_dir_.string());
}

std::filesystem::path EditorApp::SidecarPath() const {
  const std::string stem =
      std::filesystem::path(sheet_filename_).stem().string();
  return assets_dir_ / (stem + ".sprites.json");
}

std::filesystem::path EditorApp::HeaderPath() const {
  const std::string stem =
      std::filesystem::path(sheet_filename_).stem().string();
  return assets_dir_ / (ToSnakeIdentifier(stem) + ".generated.h");
}

void EditorApp::OpenSheet(const std::string& filename) {
  const auto texture_path = assets_dir_ / filename;
  if (!sheet_texture_.loadFromFile(texture_path)) {
    status_message_ = std::format("failed to load {}", texture_path.string());
    has_sheet_ = false;
    return;
  }

  sheet_filename_ = filename;
  sheet_sprite_.emplace(sheet_texture_);
  has_sheet_ = true;
  selected_ = -1;

  document_ = SheetDocument{};
  document_.texture = filename;

  const auto json_path = SidecarPath();
  if (std::filesystem::exists(json_path)) {
    auto loaded = LoadDocument(json_path);
    if (loaded) {
      document_ = std::move(*loaded);
      if (document_.texture.empty()) {
        document_.texture = filename;
      }
      status_message_ = std::format("loaded {} ({} sprites)",
                                    json_path.filename().string(),
                                    document_.sprites.size());
    } else {
      status_message_ = loaded.error();
    }
  } else {
    status_message_ = std::format("opened {} (new document)", filename);
  }

  FitView();
}

void EditorApp::Save() {
  if (!has_sheet_) {
    return;
  }

  const auto json_path = SidecarPath();
  if (auto result = SaveDocument(document_, json_path); !result) {
    status_message_ = result.error();
    return;
  }

  const std::string stem =
      std::filesystem::path(sheet_filename_).stem().string();
  const std::string header = GenerateHeader(document_, stem);
  const auto header_path = HeaderPath();
  std::ofstream out(header_path);
  if (!out) {
    status_message_ = std::format("cannot write {}", header_path.string());
    return;
  }
  out << header;
  status_message_ =
      std::format("saved {} and {}", json_path.filename().string(),
                  header_path.filename().string());
  core::LogDebug("Saved {} and {}", json_path.string(), header_path.string());
}

sf::View EditorApp::ScreenView() const {
  return sf::View(
      sf::FloatRect({0.f, 0.f}, sf::Vector2f(window_.getSize())));
}

sf::Vector2f EditorApp::MouseToWorld(sf::Vector2i pixel) const {
  return window_.mapPixelToCoords(pixel, canvas_view_);
}

void EditorApp::HandleEvent(const sf::Event& event) {
  if (event.is<sf::Event::Closed>()) {
    window_.close();
    return;
  }

  if (const auto* resized = event.getIf<sf::Event::Resized>()) {
    // SFML does not adjust views on resize. Keep a 1:1 screen view so the UI and
    // overlays are never stretched; the canvas view is rebuilt every frame in
    // DrawCanvas() from the current window size, so it follows along. Ignore the
    // zero size reported while minimised.
    if (resized->size.x != 0u && resized->size.y != 0u) {
      window_.setView(ScreenView());
      if (has_sheet_) {
        FitView();  // recenter the sheet and refit it to the new window size
      }
    }
    return;
  }

  // Always process releases so pan/draw state never gets stuck when the cursor
  // ends over an ImGui window.
  if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>()) {
    if (released->button == sf::Mouse::Button::Middle) {
      panning_ = false;
    } else if (released->button == sf::Mouse::Button::Left && drawing_) {
      drawing_ = false;
      FinishDrawnRect();
    }
    return;
  }

  const ImGuiIO& io = ImGui::GetIO();
  if (io.WantCaptureMouse || !has_sheet_) {
    return;
  }

  if (const auto* scroll = event.getIf<sf::Event::MouseWheelScrolled>()) {
    const float factor = scroll->delta > 0.f ? 1.1f : (1.f / 1.1f);
    zoom_ = std::clamp(zoom_ * factor, 0.05f, 50.f);
    return;
  }

  if (const auto* pressed = event.getIf<sf::Event::MouseButtonPressed>()) {
    if (pressed->button == sf::Mouse::Button::Middle) {
      panning_ = true;
      pan_last_pixel_ = pressed->position;
    } else if (pressed->button == sf::Mouse::Button::Left) {
      const sf::Vector2f world = MouseToWorld(pressed->position);
      if (mode_ == Mode::kDraw) {
        drawing_ = true;
        draw_start_world_ = world;
        draw_current_world_ = world;
      } else {
        PickSpriteAt(world);
      }
    }
    return;
  }

  if (const auto* moved = event.getIf<sf::Event::MouseMoved>()) {
    if (panning_) {
      const sf::Vector2i delta = moved->position - pan_last_pixel_;
      pan_last_pixel_ = moved->position;
      canvas_center_.x -= static_cast<float>(delta.x) / zoom_;
      canvas_center_.y -= static_cast<float>(delta.y) / zoom_;
    }
    if (drawing_) {
      draw_current_world_ = MouseToWorld(moved->position);
    }
    return;
  }
}

void EditorApp::PickSpriteAt(sf::Vector2f world) {
  for (int i = static_cast<int>(document_.sprites.size()) - 1; i >= 0; --i) {
    const auto& s = document_.sprites[static_cast<size_t>(i)];
    const float fx = static_cast<float>(s.x);
    const float fy = static_cast<float>(s.y);
    if (world.x >= fx && world.x < fx + static_cast<float>(s.w) &&
        world.y >= fy && world.y < fy + static_cast<float>(s.h)) {
      selected_ = i;
      SyncNameBuffer();
      return;
    }
  }
  selected_ = -1;
}

void EditorApp::FinishDrawnRect() {
  const float x0 = std::min(draw_start_world_.x, draw_current_world_.x);
  const float y0 = std::min(draw_start_world_.y, draw_current_world_.y);
  const float w = std::abs(draw_start_world_.x - draw_current_world_.x);
  const float h = std::abs(draw_start_world_.y - draw_current_world_.y);
  if (w < 2.f || h < 2.f) {
    return;
  }

  EditorSprite s;
  s.x = static_cast<int>(std::lround(x0));
  s.y = static_cast<int>(std::lround(y0));
  s.w = static_cast<int>(std::lround(w));
  s.h = static_cast<int>(std::lround(h));
  s.origin_x = static_cast<float>(s.w) / 2.f;
  s.origin_y = static_cast<float>(s.h) / 2.f;
  s.name = std::format("{}{}", static_cast<const char*>(name_prefix_),
                       document_.sprites.size());
  document_.sprites.push_back(std::move(s));
  selected_ = static_cast<int>(document_.sprites.size()) - 1;
  SyncNameBuffer();
}

void EditorApp::FitView() {
  const sf::Vector2u tex = sheet_texture_.getSize();
  const sf::Vector2u win = window_.getSize();
  const float tex_w = static_cast<float>(tex.x);
  const float tex_h = static_cast<float>(tex.y);
  if (tex_w <= 0.f || tex_h <= 0.f) {
    return;
  }
  canvas_center_ = {tex_w / 2.f, tex_h / 2.f};
  const float zx = static_cast<float>(win.x) / tex_w;
  const float zy = static_cast<float>(win.y) / tex_h;
  zoom_ = std::max(std::min(zx, zy) * 0.9f, 0.05f);
}

void EditorApp::SyncNameBuffer() {
  name_buf_[0] = '\0';
  if (selected_ < 0 || selected_ >= static_cast<int>(document_.sprites.size())) {
    return;
  }
  const std::string& name = document_.sprites[static_cast<size_t>(selected_)].name;
  const size_t count = std::min(name.size(), sizeof(name_buf_) - 1);
  std::memcpy(name_buf_, name.data(), count);
  name_buf_[count] = '\0';
}

void EditorApp::DrawCanvas() {
  const sf::Vector2u win = window_.getSize();
  canvas_view_.setSize({static_cast<float>(win.x) / zoom_,
                        static_cast<float>(win.y) / zoom_});
  canvas_view_.setCenter(canvas_center_);
  window_.setView(canvas_view_);

  if (sheet_sprite_) {
    sheet_sprite_->setPosition({0.f, 0.f});
    window_.draw(*sheet_sprite_);
  }

  const float thin = 1.f / zoom_;
  for (int i = 0; i < static_cast<int>(document_.sprites.size()); ++i) {
    const auto& s = document_.sprites[static_cast<size_t>(i)];
    sf::RectangleShape rect(
        {static_cast<float>(s.w), static_cast<float>(s.h)});
    rect.setPosition({static_cast<float>(s.x), static_cast<float>(s.y)});
    rect.setFillColor(sf::Color::Transparent);
    const bool sel = (i == selected_);
    rect.setOutlineColor(sel ? sf::Color::Yellow : sf::Color(0, 200, 255));
    rect.setOutlineThickness(sel ? thin * 2.f : thin);
    window_.draw(rect);

    if (sel) {
      const float r = 4.f * thin;
      sf::CircleShape handle(r);
      handle.setOrigin({r, r});
      handle.setFillColor(sf::Color::Red);
      handle.setPosition({static_cast<float>(s.x) + s.origin_x,
                          static_cast<float>(s.y) + s.origin_y});
      window_.draw(handle);
    }
  }

  if (drawing_) {
    const float x = std::min(draw_start_world_.x, draw_current_world_.x);
    const float y = std::min(draw_start_world_.y, draw_current_world_.y);
    const float w = std::abs(draw_start_world_.x - draw_current_world_.x);
    const float h = std::abs(draw_start_world_.y - draw_current_world_.y);
    sf::RectangleShape rect({w, h});
    rect.setPosition({x, y});
    rect.setFillColor(sf::Color(255, 255, 255, 40));
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(thin);
    window_.draw(rect);
  }
}

void EditorApp::DrawUi() {
  DrawSheetsPanel();
  if (has_sheet_) {
    DrawToolsPanel();
    DrawSpritesPanel();
  }
}

void EditorApp::DrawSheetsPanel() {
  ImGui::Begin("Sheets");
  ImGui::TextWrapped("Folder: %s", assets_dir_.string().c_str());
  if (ImGui::Button("Open folder...")) {
    folder_browser_.Open(assets_dir_);
  }
  ImGui::SameLine();
  if (ImGui::Button("Rescan")) {
    ScanAssets();
  }
  ImGui::Separator();
  for (const auto& file : png_files_) {
    const bool selected = (file == sheet_filename_);
    if (ImGui::Selectable(file.c_str(), selected)) {
      OpenSheet(file);
    }
  }
  ImGui::Separator();
  ImGui::TextWrapped("%s", status_message_.c_str());
  ImGui::End();
}

void EditorApp::DrawToolsPanel() {
  ImGui::Begin("Tools");
  ImGui::Text("Sheet: %s", sheet_filename_.c_str());
  const sf::Vector2u tex = sheet_texture_.getSize();
  ImGui::Text("Size: %u x %u", tex.x, tex.y);

  int mode = static_cast<int>(mode_);
  ImGui::RadioButton("Select", &mode, 0);
  ImGui::SameLine();
  ImGui::RadioButton("Draw", &mode, 1);
  mode_ = (mode == 1) ? Mode::kDraw : Mode::kSelect;

  if (ImGui::Button("Fit view")) {
    FitView();
  }

  ImGui::Separator();
  ImGui::Text("Grid slice");
  ImGui::InputInt("cell w", &grid_.cell_w);
  ImGui::InputInt("cell h", &grid_.cell_h);
  ImGui::InputInt("offset x", &grid_.offset_x);
  ImGui::InputInt("offset y", &grid_.offset_y);
  ImGui::InputInt("spacing x", &grid_.spacing_x);
  ImGui::InputInt("spacing y", &grid_.spacing_y);
  ImGui::InputInt("margin x", &grid_.margin_x);
  ImGui::InputInt("margin y", &grid_.margin_y);
  ImGui::InputText("name prefix", name_prefix_, sizeof(name_prefix_));

  const int tex_w = static_cast<int>(tex.x);
  const int tex_h = static_cast<int>(tex.y);
  if (ImGui::Button("Generate grid (replace)")) {
    document_.sprites = GenerateGrid(grid_, tex_w, tex_h, name_prefix_);
    selected_ = -1;
  }
  ImGui::SameLine();
  if (ImGui::Button("Append grid")) {
    auto extra = GenerateGrid(grid_, tex_w, tex_h, name_prefix_);
    document_.sprites.insert(document_.sprites.end(), extra.begin(),
                             extra.end());
  }

  ImGui::Separator();
  if (ImGui::Button("Save (JSON + header)")) {
    Save();
  }
  ImGui::End();
}

void EditorApp::DrawSpritesPanel() {
  ImGui::Begin("Sprites");
  ImGui::Text("%zu sprites", document_.sprites.size());

  ImGui::BeginChild("list", ImVec2(0.f, 200.f), true);
  for (int i = 0; i < static_cast<int>(document_.sprites.size()); ++i) {
    const auto& s = document_.sprites[static_cast<size_t>(i)];
    const bool sel = (i == selected_);
    const std::string label = std::format("{}: {}", i, s.name);
    if (ImGui::Selectable(label.c_str(), sel)) {
      selected_ = i;
      SyncNameBuffer();
    }
  }
  ImGui::EndChild();

  if (selected_ >= 0 && selected_ < static_cast<int>(document_.sprites.size())) {
    auto& s = document_.sprites[static_cast<size_t>(selected_)];
    ImGui::Separator();
    if (ImGui::InputText("name", name_buf_, sizeof(name_buf_))) {
      s.name = name_buf_;
    }
    ImGui::InputInt("x", &s.x);
    ImGui::InputInt("y", &s.y);
    ImGui::InputInt("w", &s.w);
    ImGui::InputInt("h", &s.h);
    ImGui::InputFloat("origin x", &s.origin_x);
    ImGui::InputFloat("origin y", &s.origin_y);
    if (ImGui::Button("Origin: center")) {
      s.origin_x = static_cast<float>(s.w) / 2.f;
      s.origin_y = static_cast<float>(s.h) / 2.f;
    }
    ImGui::SameLine();
    if (ImGui::Button("Origin: top-left")) {
      s.origin_x = 0.f;
      s.origin_y = 0.f;
    }
    ImGui::Separator();
    if (ImGui::Button("Delete")) {
      document_.sprites.erase(document_.sprites.begin() + selected_);
      selected_ = -1;
    }
  }
  ImGui::End();
}

}  // namespace sprite_editor
