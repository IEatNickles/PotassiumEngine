#pragma once

#include <filesystem>

using DLHandle = void *;

namespace PlatformUtils {
std::filesystem::path open_file_dialog();
std::filesystem::path save_file_dialog();

DLHandle open_dl(std::filesystem::path const &path);
DLHandle load_dl_sym(DLHandle handle, const char *sym);
void close_dl(DLHandle handle);
const char *dl_error();
}; // namespace PlatformUtils
