#pragma once

#include <filesystem>
#include <vector>

namespace KEditor {
class CodeGen {
public:
  void generate(std::vector<std::filesystem::path> const &script_files);
};
} // namespace KEditor
