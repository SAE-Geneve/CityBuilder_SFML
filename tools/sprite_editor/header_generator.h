#ifndef TOOLS_SPRITE_EDITOR_HEADER_GENERATOR_H_
#define TOOLS_SPRITE_EDITOR_HEADER_GENERATOR_H_

#include <string>
#include <string_view>

#include "sheet_document.h"

namespace sprite_editor {

// Converts an arbitrary sprite name into a valid lower_snake C++ identifier
// fragment (used for namespaces and file stems). Non-identifier characters
// collapse to '_'; a leading digit is prefixed with '_'.
std::string ToSnakeIdentifier(std::string_view raw);

// Converts an arbitrary sprite name into a PascalCase fragment used after the
// 'k' prefix for the generated constant name (e.g. "unit_tank" -> "UnitTank").
std::string ToPascalIdentifier(std::string_view raw);

// Renders the full contents of the generated header for one spritesheet.
// `sheet_stem` is the texture filename without extension; it drives the
// namespace, include guard, and header filename.
std::string GenerateHeader(const SheetDocument& document,
                           std::string_view sheet_stem);

}  // namespace sprite_editor

#endif  // TOOLS_SPRITE_EDITOR_HEADER_GENERATOR_H_
