# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build system

CMake project (>=3.30) using **vcpkg manifest mode** (`vcpkg.json`) for dependencies. C++23 (`CMAKE_CXX_STANDARD 23`, required). Pre-existing build trees: `cmake-build-debug/`, `cmake-build-release/`, `cmake-build-relwithdebinfo/`.

Two relevant CMake options, both default `OFF`:
- `ENABLE_TESTING` — pulls in GTest (vcpkg feature `testing`) and produces `core_test`, `api_test`, `game_test` executables.
- `ENABLE_PROFILING` — pulls in Tracy (vcpkg feature `profiling`) and defines `TRACY_ENABLE` publicly on `core`. Profiling code is guarded with `#ifdef TRACY_ENABLE` (see `game/src/game.cc`).

Build (debug, with tests):
```
cmake -S . -B cmake-build-debug -DENABLE_TESTING=ON -DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build cmake-build-debug
```

### CMake presets

`CMakePresets.json` defines six configure presets (Ninja, single-config) covering Debug / RelWithDebInfo / Release on Windows and Linux:

| Preset                           | `CMAKE_BUILD_TYPE` | `ENABLE_TESTING` | `ENABLE_PROFILING` |
|----------------------------------|--------------------|------------------|--------------------|
| `{windows,linux}-debug`          | `Debug`            | ON               | OFF                |
| `{windows,linux}-relwithdebinfo` | `RelWithDebInfo`   | ON               | ON                 |
| `{windows,linux}-release`        | `Release`          | OFF              | OFF                |

Build dirs are `out/build/<presetName>/`. `VCPKG_ROOT` must be set in the environment. On Windows the MSVC environment must be loaded (run from a "Developer PowerShell for VS 2022", or invoke `vcvarsall.bat x64` first) so Ninja can find `cl.exe`.

```
cmake --preset windows-debug
cmake --build --preset windows-debug
ctest --preset windows-debug          # Debug + RelWithDebInfo only
```

`CMakeUserPresets.json` is git-ignored — use it for personal overrides.

Run a single test via the GTest filter:
```
cmake-build-debug/core/core_test.exe --gtest_filter=TestSuiteName.TestName
```

The `copy_resources` custom target (CMakeLists.txt:40) wipes and re-copies `_assets/` into the build dir on every build of `city_builder_game`. Assets are loaded relative to CWD as `_assets/sprites/...` (see `AssetManager` in `core/include/assets/asset_manager.h` and `Npc::Setup`), so the game must be run from the build directory (or wherever `_assets/` lives).

## Architecture — three-layer dependency stack

Strict one-way dependencies, declared in each subdirectory's `CMakeLists.txt`:
- **`core/`** — no SFML, no game logic. Pure C++: maths (`Vec2<T>`, angle, const), behaviour-tree node primitives (`core::ai::behaviour_tree`), generic `AssetManager` template (templated on asset type, asset enum, and a `StringLiteral` folder path), `FastNoiseLite`, `LogDebug/Warning/Error` (debug-only `std::print` wrappers).
- **`api/`** — links `core` + `SFML::System/Graphics/Window`. Engine-style reusable building blocks: `TileMap` (which is a `Clickable`), `NpcManager`/`Npc`/`NpcBehaviourTree`, `ResourceManager`/`Resource`, `motion::Motor`/`Path`/`Astar`, UI (`Button`, `ButtonFactory`, `Clickable`).
- **`game/`** — links `core` + `api`. Holds the actual game in a single TU (`game/src/game.cc`) under `namespace game`. The `main.cc` at repo root just calls `game::Loop()`.

`game.cc` follows a fixed frame structure: poll events → dispatch to UI buttons and tilemap (each receives a shared `buttonsWasClicked` so tilemap clicks are suppressed when a button consumed the input) → `npc_manager_.Update(dt)` → draw tilemap, NPCs, then UI on top → `window_.display()` → `FrameMark` (Tracy).

NPC behaviour is driven by `core::ai::behaviour_tree::Node` subclasses (selector/sequence/action) composed in `NpcBehaviourTree::SetupBehaviourTree`. Each NPC currently owns its own behaviour-tree instance — see the `FIXME` in `api/include/ai/npc.h:25` about sharing one tree per role.

Pathfinding uses `api::motion::Astar::GetPath(gridStep, start, end, walkableTiles)`; walkable tiles come from `TileMap::GetWalkables()`.

## Project conventions (enforced; violating these will fail review)

This codebase has explicit written rules — read them before non-trivial edits:
- `GUIDELINES.md` — C++23, treat warnings as errors (3rd-party headers must be wrapped to silence warnings, e.g. an `sdl_include.h`-style shim), namespaces for everything, header guards (no `#pragma once`), no in-header function bodies except trivial/template, no Singletons (use namespace + free functions; if interface inheritance is needed, define impl in `.cc`), use `std::expected` for expected errors and exceptions for unexpected ones, `std::optional` only for optional values (not error handling), prefer signed integers and `std::ssize`, use `float` (not double).
- `FORBIDDEN_FEATURES.md` — banned in this codebase: `std::shared_ptr`/`weak_ptr`, `std::regex`, `std::bind`, `std::function`, `std::any`, `std::list`/`forward_list`, raw `new`/`delete`/`malloc`/`free` (use `unique_ptr`), C++20 modules, parallel STL, `mutable` (except `std::mutex` member), `const_cast`, C-style casts, `std::lerp`, `std::vector<bool>`, coroutines, RTTI/`dynamic_cast`, `using namespace`. Recent commits show active migration away from `std::function` and `std::string` where avoidable.

Naming/style observed in code: classes `PascalCase`, methods `PascalCase` starting with a verb (`Setup`, `Update`, `Draw`, `HandleEvent`, `GetCollectibles`), private members trailing underscore (`tilemap_`, `npc_manager_`), enums prefixed `k` (`Tile::kBg`, `NpcType::kBlueWood`, `Status::kSuccess`).
