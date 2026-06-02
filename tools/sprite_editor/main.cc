#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

#include "editor_app.h"
#include "header_generator.h"
#include "sheet_document.h"

namespace {

// Headless mode: regenerate the .h for every "<sheet>.sprites.json" sidecar in
// the assets directory, without opening the GUI. Useful for build pipelines.
int RunRegen(const std::filesystem::path& assets_dir,
             const std::filesystem::path& generated_dir) {
  namespace fs = std::filesystem;
  constexpr std::string_view kSuffix = ".sprites.json";

  std::error_code ec;
  fs::create_directories(generated_dir, ec);

  int count = 0;
  for (const auto& entry : fs::directory_iterator(assets_dir, ec)) {
    const std::string name = entry.path().filename().string();
    if (!std::string_view(name).ends_with(kSuffix)) {
      continue;
    }

    auto document = sprite_editor::LoadDocument(entry.path());
    if (!document) {
      std::cerr << document.error() << '\n';
      continue;
    }

    const std::string stem = name.substr(0, name.size() - kSuffix.size());
    const std::string header = sprite_editor::GenerateHeader(*document, stem);
    const auto out_path =
        generated_dir / (sprite_editor::ToSnakeIdentifier(stem) + ".h");

    std::ofstream out(out_path);
    if (!out) {
      std::cerr << "cannot write " << out_path.string() << '\n';
      continue;
    }
    out << header;
    std::cout << "generated " << out_path.string() << '\n';
    ++count;
  }

  std::cout << "regenerated " << count << " header(s)\n";
  return 0;
}

}  // namespace

int main(int argc, char** argv) {
  if (argc >= 2 && std::string_view(argv[1]) == "--regen") {
    const std::filesystem::path assets =
        (argc >= 3) ? std::filesystem::path(argv[2])
                    : std::filesystem::path("_assets/sprites");
    const std::filesystem::path generated =
        (argc >= 4) ? std::filesystem::path(argv[3])
                    : std::filesystem::path("api/include/graphics/generated");
    return RunRegen(assets, generated);
  }

  sprite_editor::EditorApp app;
  app.Run();
  return 0;
}
