#include "utils/PlatformUtils.hpp"
#include <dlfcn.h>

namespace PlatformUtils {
std::filesystem::path open_file_dialog() { return "Unsupported"; }

std::filesystem::path save_file_dialog() { return "Unsupported"; }

DLHandle open_dl(std::filesystem::path const &path) {
  return dlopen(path.c_str(), RTLD_LAZY);
}

DLHandle load_dl_sym(DLHandle handle, const char *sym) {
  return dlsym(handle, sym);
}

void close_dl(DLHandle handle) { dlclose(handle); }

const char *dl_error() { return dlerror(); }
} // namespace PlatformUtils
