#pragma once

#include <filesystem>
#include <string>

#include "nlohmann/json.hpp"

struct Project {
  Project() = default;
  Project(std::filesystem::path const &path);
  Project(nlohmann::json const &json);

  std::string name = "Project";
  std::filesystem::path root_directory = "~";
  std::filesystem::path asset_directory = "Assets";
  std::filesystem::path build_directory = "build";
};
