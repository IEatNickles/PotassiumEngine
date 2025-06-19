#pragma once

#include "imgui.h"

#include "nlohmann/json.hpp"

struct EditorThemeSizes {
  int FrameRounding;
  int ScrollbarRounding;
  int GrabRounding;
  ImVec2 WindowPadding;
  int ScrollbarSize;
  int TabRoundin;
  ImGuiTreeNodeFlags TreeLinesFlags;
  int TreeLinesSize;
  int TreeLinesRounding;
  ImVec2 WindowTitleAlign;
  ImGuiDir WindowMenuButtonPosition;
  int DockingSeparatorSize;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(EditorThemeSizes, FrameRounding,
                                 ScrollbarRounding, GrabRounding, WindowPadding,
                                 ScrollbarSize, TabRoundin, TreeLinesFlags,
                                 TreeLinesSize, TreeLinesRounding,
                                 WindowTitleAlign, WindowMenuButtonPosition,
                                 DockingSeparatorSize)
};

struct EditorThemeColors {
  ImColor Text;
  ImColor TextDisabled;
  ImColor TextLink;
  ImColor WindowBg;
  ImColor ChildBg;
  ImColor PopupBg;
  ImColor Border;
  ImColor BorderShadow;
  ImColor FrameBg;
  ImColor FrameBgHovered;
  ImColor FrameBgActive;
  ImColor TitleBg;
  ImColor TitleBgActive;
  ImColor TitleBgCollapsed;
  ImColor MenuBarBg;
  ImColor ScrollbarBg;
  ImColor ScrollbarGrab;
  ImColor ScrollbarGrabHovered;
  ImColor ScrollbarGrabActive;
  ImColor CheckMark;
  ImColor SliderGrab;
  ImColor SliderGrabActive;
  ImColor Button;
  ImColor ButtonHovered;
  ImColor ButtonActive;
  ImColor Header;
  ImColor HeaderHovered;
  ImColor HeaderActive;
  ImColor Separator;
  ImColor SeparatorHovered;
  ImColor SeparatorActive;
  ImColor ResizeGrip;
  ImColor ResizeGripHovered;
  ImColor ResizeGripActive;
  ImColor Tab;
  ImColor TabHovered;
  ImColor TabActive;
  ImColor TabUnfocused;
  ImColor TabUnfocusedActive;
  ImColor TabSelectedOverline;
  ImColor DockingPreview;
  ImColor DockingEmptyBg;
  ImColor PlotLines;
  ImColor PlotLinesHovered;
  ImColor PlotHistogram;
  ImColor PlotHistogramHovered;
  ImColor TableHeaderBg;
  ImColor TableBorderStrong;
  ImColor TableBorderLight;
  ImColor TableRowBg;
  ImColor TableRowBgAlt;
  ImColor TextSelectedBg;
  ImColor DragDropTarget;
  ImColor NavHighlight;
  ImColor NavWindowingHighlight;
  ImColor NavWindowingDimBg;
  ImColor ModalWindowDimBg;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(
      EditorThemeColors, Text, TextDisabled, TextLink, WindowBg, ChildBg,
      PopupBg, Border, BorderShadow, FrameBg, FrameBgHovered, FrameBgActive,
      TitleBg, TitleBgActive, TitleBgCollapsed, MenuBarBg, ScrollbarBg,
      ScrollbarGrab, ScrollbarGrabHovered, ScrollbarGrabActive, CheckMark,
      SliderGrab, SliderGrabActive, Button, ButtonHovered, ButtonActive, Header,
      HeaderHovered, HeaderActive, Separator, SeparatorHovered, SeparatorActive,
      ResizeGrip, ResizeGripHovered, ResizeGripActive, Tab, TabHovered,
      TabActive, TabUnfocused, TabUnfocusedActive, TabSelectedOverline,
      DockingPreview, DockingEmptyBg, PlotLines, PlotLinesHovered,
      PlotHistogram, PlotHistogramHovered, TableHeaderBg, TableBorderStrong,
      TableBorderLight, TableRowBg, TableRowBgAlt, TextSelectedBg,
      DragDropTarget, NavHighlight, NavWindowingHighlight, NavWindowingDimBg,
      ModalWindowDimBg);
};

struct EditorTheme {
  EditorThemeSizes sizes;
  EditorThemeColors colors;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(EditorTheme, sizes, colors);
};
