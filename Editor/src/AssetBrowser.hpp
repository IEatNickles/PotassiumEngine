#pragma once

#include <filesystem>

#include "imgui.h"

class AssetBrowser {
public:
  AssetBrowser() = default;

  void set_root_directory(std::filesystem::path const &path) {
    m_root_directory = path;
    if (!std::filesystem::exists(m_root_directory / m_current_directory)) {
      m_current_directory = ".";
    }
  }

  void draw() {
    ImGui::Begin("Asset Browser");

    // ImGui::BeginDisabled(std::filesystem::equivalent(
    //     m_current_directory, m_current_directory.parent_path()));
    if (ImGui::Button("")) {
      m_current_directory = m_current_directory.parent_path();
    }
    // ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::Text("%s", m_current_directory.c_str());
    ImGui::SliderInt("󰕭", &m_items_per_row, 1, 25);
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(8, 8));
    float button_size = (ImGui::GetWindowWidth() / (float)m_items_per_row) - 16;
    if (ImGui::BeginTable("Content", m_items_per_row)) {
      int i = 0;
      for (auto f : std::filesystem::directory_iterator(m_root_directory /
                                                        m_current_directory)) {
        ImGui::TableNextColumn();
        ImGui::PushID(i);
        std::string icon = "\uf15b";
        if (f.is_directory()) {
          icon = "\uf4d3";
        } else {
          if (f.path().extension() == ".cpp" ||
              f.path().extension() == ".cxx" || f.path().extension() == ".cc") {
            icon = "\ue61d";
          } else if (f.path().extension() == ".hpp" ||
                     f.path().extension() == ".h" ||
                     f.path().extension() == ".hh") {
            icon = "\uf0fd";
          } else if (f.path().extension() == ".png" ||
                     f.path().extension() == ".jpg" ||
                     f.path().extension() == ".jpeg" ||
                     f.path().extension() == ".webp") {
            icon = "\uf03e";
          }
        }
        ImGui::PushFontSize(button_size * 0.5f);
        if (ImGui::Button(icon.c_str(), ImVec2(button_size, button_size))) {
          if (f.is_directory()) {
            m_current_directory /= f.path().filename();
          }
        }
        ImGui::PopFontSize();
        if (ImGui::IsItemHovered()) {
          ImGui::SetTooltip("%s", f.path().filename().c_str());
        }

        ImGui::Text("%s", f.path().filename().c_str());
        ImGui::PopID();
        ++i;
      }
      ImGui::EndTable();
    }
    ImGui::PopStyleVar();

    ImGui::End();
  }

private:
  std::filesystem::path m_root_directory = ".";
  std::filesystem::path m_current_directory = "";

  int m_items_per_row = 10;
};
