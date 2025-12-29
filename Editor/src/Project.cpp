#include "Project.hpp"

#include <fstream>

#include "Log.hpp"

Project::Project(std::filesystem::path const &path) {
  std::ifstream ifs(path);
  nlohmann::json j;
  ifs >> j;

  DEBUG_ASSERT(j.contains("root_directory"),
               "Project must contain a root directory");
  root_directory = j["root_directory"].get<std::string>();

  DEBUG_ASSERT(j.contains("project_name"), "Project must contain a name");
  name = j["project_name"].get<std::string>();

  if (j.contains("assets_directory")) {
    asset_directory = root_directory / j["assets_directory"].get<std::string>();
  } else {
    DEBUG_WARN("KProj file did not contain an asset directory, defaulting to "
               "{}/Assets",
               root_directory.string());
    build_directory = root_directory / "Assets";
  }
  if (j.contains("build_directory")) {
    build_directory = root_directory / j["build_directory"].get<std::string>();
  } else {
    DEBUG_WARN("KProj file did not contain a build directory, defaulting to "
               "{}/build",
               root_directory.string());
    build_directory = root_directory / "build";
  }

  DEBUG_TRACE("root: {}, assets: {}, build: {}", root_directory.string(),
              asset_directory.string(), build_directory.string());

  ifs.close();
}

Project::Project(nlohmann::json const &j) {
  if (j.contains("asset_directory")) {
    asset_directory = j["asset_directory"].get<std::string>();
  }
  if (j.contains("asset_directory")) {
    build_directory = j["build_directory"].get<std::string>();
  }
  if (j.contains("project_name")) {
    name = j["project_name"].get<std::string>();
  }
}
