#include "AssetBrowser.hpp"
#include "Project.hpp"
#include "Viewport.hpp"
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/vector_float3.hpp"
#include "scripting/CodeGen.hpp"
#include "tinyfiledialogs.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <Log.hpp>

#include <Application.hpp>

#include <cstdint>
#include <cstdlib>
#include <filesystem>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

#include <imgui.h>
#include <iostream>

#include <Scene.hpp>
#include <vector>

#include "components/Camera.hpp"
#include "components/MeshRenderer.hpp"
#include "components/Name.hpp"
#include "components/Relationship.hpp"
#include "components/Transform.hpp"

#include "serialize/Components.hpp"

template <typename T> void draw(T &);

#define _NUM_ARGS(                                                             \
    v255, v254, v253, v252, v251, v250, v249, v248, v247, v246, v245, v244,    \
    v243, v242, v241, v240, v239, v238, v237, v236, v235, v234, v233, v232,    \
    v231, v230, v229, v228, v227, v226, v225, v224, v223, v222, v221, v220,    \
    v219, v218, v217, v216, v215, v214, v213, v212, v211, v210, v209, v208,    \
    v207, v206, v205, v204, v203, v202, v201, v200, v199, v198, v197, v196,    \
    v195, v194, v193, v192, v191, v190, v189, v188, v187, v186, v185, v184,    \
    v183, v182, v181, v180, v179, v178, v177, v176, v175, v174, v173, v172,    \
    v171, v170, v169, v168, v167, v166, v165, v164, v163, v162, v161, v160,    \
    v159, v158, v157, v156, v155, v154, v153, v152, v151, v150, v149, v148,    \
    v147, v146, v145, v144, v143, v142, v141, v140, v139, v138, v137, v136,    \
    v135, v134, v133, v132, v131, v130, v129, v128, v127, v126, v125, v124,    \
    v123, v122, v121, v120, v119, v118, v117, v116, v115, v114, v113, v112,    \
    v111, v110, v109, v108, v107, v106, v105, v104, v103, v102, v101, v100,    \
    v99, v98, v97, v96, v95, v94, v93, v92, v91, v90, v89, v88, v87, v86, v85, \
    v84, v83, v82, v81, v80, v79, v78, v77, v76, v75, v74, v73, v72, v71, v70, \
    v69, v68, v67, v66, v65, v64, v63, v62, v61, v60, v59, v58, v57, v56, v55, \
    v54, v53, v52, v51, v50, v49, v48, v47, v46, v45, v44, v43, v42, v41, v40, \
    v39, v38, v37, v36, v35, v34, v33, v32, v31, v30, v29, v28, v27, v26, v25, \
    v24, v23, v22, v21, v20, v19, v18, v17, v16, v15, v14, v13, v12, v11, v10, \
    v9, v8, v7, v6, v5, v4, v3, v2, v1, N, ...)                                \
  N

#define NUM_ARGS(...)                                                          \
  _NUM_ARGS(                                                                   \
      __VA_ARGS__, 255, 254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, \
      243, 242, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232, 231, 230,    \
      229, 228, 227, 226, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216,    \
      215, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202,    \
      201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188,    \
      187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174,    \
      173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160,    \
      159, 158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146,    \
      145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132,    \
      131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118,    \
      117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104,    \
      103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87,  \
      86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69,  \
      68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51,  \
      50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33,  \
      32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15,  \
      14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define EXPAND(x) x
#define FIRST_ARG(x, ...) (x)
#define REST_OF_ARGS(x, ...) (__VA_ARGS__)
#define FOREACH(M, list) FOREACH_(NUM_ARGS list, M, list)
#define FOREACH_(N, M, list) FOREACH__(N, M, list)
#define FOREACH__(N, M, list) FOREACH_##N(M, list)
#define FOREACH_1(M, list) M list
#define FOREACH_2(M, list)                                                     \
  EXPAND(M FIRST_ARG list) FOREACH_1(M, REST_OF_ARGS list)
#define FOREACH_3(M, list)                                                     \
  EXPAND(M FIRST_ARG list) FOREACH_2(M, REST_OF_ARGS list)

#define STRINGIFY(x) #x,
#define LIST_OF_STRINGS(...) {FOREACH(STRINGIFY, (__VA_ARGS__))}

#define SERIALIZE_BEGIN(T) template <> void draw(T &component) {

#define _FIELD_ASSERT_PARAM_IS(name, T)                                        \
  static_assert(std::is_same_v<decltype(component.name), T>,                   \
                "the type of the field is incorrect");
#define _FIELD_ASSERT_PARAM_IS2(name, T, T1)                                   \
  static_assert(std::is_same_v<decltype(component.name), T> ||                 \
                    std::is_same_v<decltype(component.name), T2>,              \
                "the type of the field is incorrect");

#define FIELD_FLOAT(name)                                                      \
  _FIELD_ASSERT_PARAM_IS(name, float)                                          \
  ImGui::DragFloat(#name, &component.name);
#define FIELD_FLOAT2(name)                                                     \
  _FIELD_ASSERT_PARAM_IS(name, glm::vec2)                                      \
  ImGui::DragFloat2(#name, &component.name[0]);
#define FIELD_FLOAT3(name)                                                     \
  _FIELD_ASSERT_PARAM_IS(name, glm::vec3)                                      \
  ImGui::DragFloat3(#name, &component.name[0]);
#define FIELD_FLOAT4(name)                                                     \
  _FIELD_ASSERT_PARAM_IS(name, glm::vec4)                                      \
  ImGui::DragFloat4(#name, &component.name[0]);

#define FIELD_INT(name)                                                        \
  _FIELD_ASSERT_PARAM_IS(name, int)                                            \
  ImGui::DragInt(#name, &component.name);
#define FIELD_INT2(name)                                                       \
  _FIELD_ASSERT_PARAM_IS(name, glm::ivec2)                                     \
  ImGui::DragInt2(#name, &component.name[0]);
#define FIELD_INT3(name)                                                       \
  _FIELD_ASSERT_PARAM_IS(name, glm::ivec3)                                     \
  ImGui::DragInt3(#name, &component.name[0]);
#define FIELD_INT4(name)                                                       \
  _FIELD_ASSERT_PARAM_IS(name, glm::ivec4)                                     \
  ImGui::DragInt4(#name, &component.name[0]);

#define FIELD_STRING(name)                                                     \
  _FIELD_ASSERT_PARAM_IS2(name, std::string, const char *);                    \
  ImGui::InputText(#name, &component.name);

#define FIELD_SLIDER_FLOAT(name, min, max)                                     \
  ImGui::SliderFloat(#name, &component.name, min, max);
#define FIELD_SLIDER_INT(name, min, max)                                       \
  ImGui::SliderInt(#name, &component.name, min, max);
#define FIELD_DROPDOWN(name, ...)                                              \
  {                                                                            \
    char const *const items[NUM_ARGS(__VA_ARGS__)] =                           \
        LIST_OF_STRINGS(__VA_ARGS__);                                          \
    int c = component.name;                                                    \
    if (ImGui::Combo(#name, (int *)&c, items, NUM_ARGS(__VA_ARGS__))) {        \
      component.name = (decltype(component.name))c;                            \
    }                                                                          \
  }
#define FIELD_COLOR3(name)                                                     \
  static_assert(std::is_same_v<decltype(component.name), glm::vec3>);          \
  ImGui::ColorPicker3(#name, &component.name[0]);
#define FIELD_COLOR4(name)                                                     \
  static_assert(std::is_same_v<decltype(component.name), glm::vec4>);          \
  ImGui::ColorPicker4(#name, &component.name[0]);

#define FIELD_CUSTOM(fn) fn

#define SERIALIZE_END }

SERIALIZE_BEGIN(CameraComponent);
FIELD_FLOAT(size);
FIELD_DROPDOWN(projection, Orthographic, Perspective)
FIELD_SLIDER_FLOAT(aspect, 0, 1);
FIELD_COLOR3(clear_color);
FIELD_FLOAT3(clear_color);
FIELD_CUSTOM({
  ImGui::Text("HI");
  ImGui::Text("I got size: %f", component.size);
})
SERIALIZE_END

void glfw_error_callback(int error_code, char const *description) {
  std::cout << "GLFW Error: " << error_code << ": " << description << "\n";
}

void gl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                         GLsizei length, const GLchar *message,
                         const void *userParam) {
  std::stringstream ss;
  ss << "OPENGL ";
  switch (source) {
  case GL_DEBUG_SOURCE_API:
    ss << "API";
    break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    ss << "Window System";
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    ss << "Shader Compiler";
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    ss << "Third Party";
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    ss << "Application";
    break;
  case GL_DEBUG_SOURCE_OTHER:
    ss << "Other";
    break;
  }
  ss << " (";
  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    ss << "Error";
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    ss << "Deprecated Behavior";
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    ss << "Undefined Behavior";
    break;
  case GL_DEBUG_TYPE_PORTABILITY:
    ss << "Portability";
    break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    ss << "Performance";
    break;
  case GL_DEBUG_TYPE_MARKER:
    ss << "Marker";
    break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
    ss << "Push Group";
    break;
  case GL_DEBUG_TYPE_POP_GROUP:
    ss << "Pop Group";
    break;
  case GL_DEBUG_TYPE_OTHER:
    ss << "Other";
    break;
  }
  ss << "): " << message;

  switch (severity) {
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    DEBUG_TRACE("{}", ss.str());
    break;
  case GL_DEBUG_SEVERITY_LOW:
  case GL_DEBUG_SEVERITY_MEDIUM:
    DEBUG_WARN("{}", ss.str());
    break;
  case GL_DEBUG_SEVERITY_HIGH:
    DEBUG_ERROR("{}", ss.str());
    break;
  }
}

void colors();

class EditorApp : public KEngine::App {
public:
  virtual void run() override {
    glfwSetErrorCallback(glfw_error_callback);

    glfwInit();
    GLFWwindow *window =
        glfwCreateWindow(1280, 720, "Potassium", nullptr, nullptr);
    glfwMaximizeWindow(window);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(gl_message_callback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                          GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    io.Fonts->AddFontFromFileTTF(
        "Editor/assets/fonts/JetBrainsMonoNerdFont-Regular.ttf", 16);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    colors();

    glEnable(GL_DEPTH_TEST);

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glGenTextures(1, &m_fb_tex);
    glBindTexture(GL_TEXTURE_2D, m_fb_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1920, 1080, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           m_fb_tex, 0);

    glGenTextures(1, &m_fb_depth_tex);
    glBindTexture(GL_TEXTURE_2D, m_fb_depth_tex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, 1920, 1080);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                           GL_TEXTURE_2D, m_fb_depth_tex, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    bool show_imgui_example_window = false;

    m_current_scene = new KEngine::Scene();

    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      ImGui::DockSpaceOverViewport();

      if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
          if (ImGui::MenuItem("Save As...")) {
            char *file =
                tinyfd_saveFileDialog("Save File", "~/", 0, nullptr, nullptr);
            if (file != nullptr) {
              DEBUG_TRACE("saving file: {}", file);
              m_current_scene->save(file);
            }
          }
          if (ImGui::MenuItem("Save")) {
            m_current_scene->save();
          }
          if (ImGui::MenuItem("Open")) {
            char *file = tinyfd_openFileDialog("Open File", "~/", 0, nullptr,
                                               nullptr, 0);
            if (file != nullptr) {
              DEBUG_TRACE("opening file: {}", file);
              auto *scene = new KEngine::Scene();
              scene->load(file);

              if (m_current_scene) {
                delete m_current_scene;
              }
              m_current_scene = scene;
            }
          }
          if (ImGui::MenuItem("Open Project")) {
            char *file = tinyfd_openFileDialog("Open Project", "~/", 0, nullptr,
                                               nullptr, 0);
            if (file != nullptr) {
              DEBUG_TRACE("opening project: {}", file);
              m_project = Project(std::filesystem::path(file));
              m_asset_browser.set_root_directory(m_project.asset_directory);
            }
          }
          if (!show_imgui_example_window &&
              ImGui::MenuItem("Show ImGui Example")) {
            show_imgui_example_window = true;
          }
          if (show_imgui_example_window &&
              ImGui::MenuItem("Hide ImGui Example")) {
            show_imgui_example_window = false;
          }
          if (ImGui::MenuItem("Compile Scripts")) {
            KEditor::CodeGen cg;
            std::vector<std::filesystem::path> scripts = {};
            for (auto f : std::filesystem::recursive_directory_iterator(
                     "Sandbox/assets")) {
              if (f.is_regular_file() && f.path().extension() == ".hpp") {
                scripts.push_back(f.path());
              }
            }
            cg.generate(scripts);
            system("cmake -S ./generated/ -B ./generated/build/");
            system("cmake --build ./generated/build/");
          }
          ImGui::EndMenu();
        }
        if (ImGui::BeginPopup("OpenFileDialog")) {
          ImGui::Text("wut up");
          ImGui::EndPopup();
        }
        ImGui::EndMainMenuBar();
      }

      if (show_imgui_example_window) {
        ImGui::ShowDemoWindow();
      }

      ImGui::Begin("Scene");

      if (ImGui::Button("Add Entity")) {
        m_selected_entity = m_current_scene->entity();
      }

      if (m_current_scene) {
        bool open = ImGui::TreeNodeEx(m_current_scene->name().c_str(),
                                      ImGuiTreeNodeFlags_DefaultOpen |
                                          ImGuiTreeNodeFlags_OpenOnArrow |
                                          ImGuiTreeNodeFlags_Framed);
        if (ImGui::BeginPopupContextItem()) {
          if (ImGui::MenuItem("New Entity")) {
            m_selected_entity = m_current_scene->entity();
          }
          ImGui::EndPopup();
        }
        if (open) {
          m_current_scene->ecs().view<entt::entity>().each(
              [&](entt::entity e) { draw_entity(e); });
          ImGui::TreePop();
        }
      }

      ImGui::End(); // Scene
      ImGui::Begin("Game");

      glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
      glViewport(0, 0, 1920, 1080);
      m_current_scene->update();
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      int w = ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2;
      int h = w * 9 / 16;
      ImGui::Image((ImTextureID)(intptr_t)m_fb_tex, ImVec2(w, h), ImVec2(0, 1),
                   ImVec2(1, 0));
      ImGui::End(); // Game

      ImGui::Begin("EntityView");

      if (m_selected_entity != entt::null) {
        if (ImGui::InputText("Name", &name)) {
          NameComponent *c =
              m_current_scene->try_get<NameComponent>(m_selected_entity);
          if (!c) {
            m_current_scene->set<NameComponent>(m_selected_entity, name);
          } else {
            c->name = name;
          }
        }
        draw_components(m_selected_entity);
      }

      ImGui::End(); // EntityView

      m_asset_browser.draw();

      ImGui::Render();

      int display_w, display_h;
      glfwGetFramebufferSize(window, &display_w, &display_h);
      glViewport(0, 0, display_w, display_h);
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow *backup_current_window = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_window);
      }

      glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    glfwTerminate();
  }

  void draw_entity(entt::entity e, entt::entity parent = entt::null) {
    if (auto r = m_current_scene->try_get<RelationshipComponent>(e)) {
      if (r->parent != entt::null && r->parent != parent) {
        return;
      }
    }
    ImGui::PushID((uint64_t)e);
    bool has_children = false;
    if (auto h = m_current_scene->try_get<RelationshipComponent>(e); h) {
      has_children = h->child_count > 0;
    }
    ImGuiTreeNodeFlags flags =
        ((e == m_selected_entity) ? ImGuiTreeNodeFlags_Selected : 0) |
        ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth |
        ImGuiTreeNodeFlags_OpenOnArrow |
        (has_children ? 0 : ImGuiTreeNodeFlags_Leaf);

    std::string name = std::to_string((uint64_t)e);
    if (auto c = m_current_scene->try_get<NameComponent>(e)) {
      name = c->name;
    }
    bool open = ImGui::TreeNodeEx(name.c_str(), flags);
    bool delete_entity = false;

    if (ImGui::BeginPopupContextItem()) {
      delete_entity = ImGui::MenuItem("Delete");
      if (ImGui::MenuItem("Duplicate")) {
        entt::entity dst = m_current_scene->entity();
        for (auto [id, storage] : m_current_scene->ecs().storage()) {
          if (storage.contains(e)) {
            if (storage.contains(e)) {
              storage.remove(dst);
            }
            storage.push(dst, storage.value(e));
          }
        }
        m_selected_entity = dst;
      }
      ImGui::MenuItem("Copy");
      ImGui::MenuItem("Cut");
      if (ImGui::MenuItem("New Child")) {
        entt::entity child = m_current_scene->entity();
        m_current_scene->get_or_add<RelationshipComponent>(e).add_child(
            m_current_scene->ecs(), e, child);
      }
      ImGui::EndPopup();
    }

    if (ImGui::BeginDragDropSource()) {
      ImGui::SetDragDropPayload("ENTITY_CHILD", &e, sizeof(entt::entity));
      ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget()) {
      if (auto payload = ImGui::AcceptDragDropPayload("ENTITY_CHILD")) {
        entt::entity child = *(entt::entity *)payload->Data;
        RelationshipComponent &r =
            m_current_scene->get_or_add<RelationshipComponent>(e);
        RelationshipComponent &cr =
            m_current_scene->get_or_add<RelationshipComponent>(child);

        if (cr.parent == e) {
          goto end;
        }
        if (!r.add_child(m_current_scene->ecs(), e, child)) {
          goto end;
        }
      }
    end:
      ImGui::EndDragDropTarget();
    }

    if (ImGui::IsItemClicked()) {
      m_selected_entity = e;
      this->name = name;
    }
    if (open) {
      if (auto r = m_current_scene->try_get<RelationshipComponent>(e)) {
        if (r->child_count > 0) {
          entt::entity curr = r->first;
          for (size_t i = 0; i < r->child_count; ++i) {
            draw_entity(curr, e);
            curr = m_current_scene->get<RelationshipComponent>(curr).next;
          }
        }
      }
      ImGui::TreePop();
    }
    ImGui::PopID();

    if (delete_entity) {
      if (m_selected_entity == e) {
        m_selected_entity = entt::null;
      }
      m_current_scene->ecs().destroy(e);
    }
  }

  template <typename T, typename Fn>
  void draw_component(std::string const &name, entt::entity e, Fn draw_fn) {
    // if (m_current_scene->has<T>(e)) {
    //   static constexpr char const *button_label = "...";
    //   ImVec2 button_size = ImGui::CalcTextSize(button_label);

    //   bool open =
    //       ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Framed |
    //                                           ImGuiTreeNodeFlags_FramePadding
    //                                           |
    //                                           ImGuiTreeNodeFlags_DefaultOpen
    //                                           |
    //                                           ImGuiTreeNodeFlags_AllowOverlap);
    //   ImGui::SameLine();

    //   ImGui::SetCursorPosX(ImGui::GetWindowWidth() - button_size.x - 16.0f);
    //   ImGui::PushID(typeid(T).name());
    //   bool remove = false;
    //   if (ImGui::Button(button_label)) {
    //     ImGui::OpenPopup("Options");
    //   }
    //   if (ImGui::BeginPopup("Options")) {
    //     remove = ImGui::Button("Remove");
    //     ImGui::EndPopup();
    //   }
    //   ImGui::PopID();
    //   if (open) {
    //     ImGui::PushID((int)e);
    //     ImGui::PopID();

    //     nlohmann::json j = m_current_scene->get<T>(e);
    //     for (auto &[k, v] : j.items()) {
    //       if (v.is_number_integer()) {
    //         int s = v.get<int>();
    //         ImGui::DragInt(k.c_str(), &s);
    //         v = s;
    //       } else if (v.is_number_float()) {
    //         float s = v.get<float>();
    //         ImGui::DragFloat(k.c_str(), &s);
    //         v = s;
    //       } else if (v.is_string()) {
    //         std::string s = v.get<std::string>();
    //         ImGui::InputText(k.c_str(), &s);
    //         v = s;
    //       } else if (v.is_array()) {
    //         if (v.size() == 2) {
    //           if (v[0].is_number_float() && v[1].is_number_float()) {
    //             float vec[2];
    //             vec[0] = v[0].get<float>();
    //             vec[1] = v[1].get<float>();
    //             ImGui::DragFloat2(k.c_str(), vec);
    //             v = vec;
    //           }
    //         } else if (v.size() == 3) {
    //           if (v[0].is_number_float() && v[1].is_number_float() &&
    //               v[2].is_number_float()) {
    //             float vec[3];
    //             vec[0] = v[0].get<float>();
    //             vec[1] = v[1].get<float>();
    //             vec[2] = v[2].get<float>();
    //             ImGui::DragFloat3(k.c_str(), vec);
    //             v = vec;
    //           }
    //         } else if (v.size() == 4) {
    //           if (v[0].is_number_float() && v[1].is_number_float() &&
    //               v[2].is_number_float() && v[3].is_number_float()) {
    //             float vec[4];
    //             vec[0] = v[0].get<float>();
    //             vec[1] = v[1].get<float>();
    //             vec[2] = v[2].get<float>();
    //             vec[3] = v[3].get<float>();
    //             ImGui::DragFloat4(k.c_str(), vec);
    //             v = vec;
    //           }
    //         }
    //       }
    //     }
    //     m_current_scene->get<T>(e) = j;

    //     ImGui::TreePop();
    //   }
    //   if (remove) {
    //     m_current_scene->remove_component<T>(e);
    //   }
    // }
    if (m_current_scene->has<T>(e)) {
      static constexpr char const *button_label = "...";
      ImVec2 button_size = ImGui::CalcTextSize(button_label);

      bool open =
          ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Framed |
                                              ImGuiTreeNodeFlags_FramePadding |
                                              ImGuiTreeNodeFlags_DefaultOpen |
                                              ImGuiTreeNodeFlags_AllowOverlap);
      ImGui::SameLine();

      ImGui::SetCursorPosX(ImGui::GetWindowWidth() - button_size.x - 16.0f);
      ImGui::PushID(typeid(T).name());
      bool remove = false;
      if (ImGui::Button(button_label)) {
        ImGui::OpenPopup("Options");
      }
      if (ImGui::BeginPopup("Options")) {
        remove = ImGui::Button("Remove");
        ImGui::EndPopup();
      }
      ImGui::PopID();
      if (open) {
        ImGui::PushID((int)e);
        T &c = m_current_scene->get<T>(e);
        draw_fn(c);
        ImGui::PopID();

        ImGui::TreePop();
      }
      if (remove) {
        m_current_scene->remove_component<T>(e);
      }
    }
  }

  template <typename T>
  void draw_add_component(std::string const &name, entt::entity e) {
    if (m_current_scene->has<T>(e))
      return;
    if (ImGui::MenuItem(name.c_str())) {
      m_current_scene->set<T>(e);
    }
  }

  void draw_components(entt::entity e) {

    draw_component<RelationshipComponent>(
        "Relationship", e, [](RelationshipComponent &r) {
          ImGui::Text("count: %zu", (uint64_t)r.child_count);
          ImGui::Text("first: %zu", (uint64_t)r.first);
          ImGui::Text("last: %zu", (uint64_t)r.last);
          ImGui::Text("prev: %zu", (uint64_t)r.prev);
          ImGui::Text("next: %zu", (uint64_t)r.next);
          ImGui::Text("parent: %zu", (uint64_t)r.parent);
        });

    draw_component<TransformComponent>(
        "Transform", e, [](TransformComponent &t) {
          glm::vec3 pos = t.position;
          if (ImGui::DragFloat3("Position", &pos[0], 0.1f)) {
            t.position = pos;
          }

          glm::vec3 euler = glm::degrees(glm::eulerAngles(t.rotation));
          ImGui::DragFloat3("Euler Angles", &euler[0], 0.1f);
          if (ImGui::IsItemDeactivatedAfterEdit()) {
            t.rotation = glm::radians(euler);
          }

          glm::vec3 scale = t.scale;
          if (ImGui::DragFloat3("Scale", &scale[0], 0.1f)) {
            t.scale = scale;
          }
        });

    draw_component<CameraComponent>("Camera", e, [](CameraComponent &c) {
      char const *const items[2] = {"Orthographic", "Perspective"};

      ProjectionType proj = c.projection;
      if (ImGui::Combo("Projection", (int *)&proj, items, 2)) {
        c.projection = proj;
      }

      float size = c.size;
      switch (c.projection) {
      case PROJ_ORTHOGRAPHIC:
        if (ImGui::DragFloat("Size", &size, 0.1f)) {
          c.size = size;
        }
        break;
      case PROJ_PERSPECTIVE:
        if (ImGui::DragFloat("Field Of View", &size)) {
          c.size = size;
        }
        break;
      }
      float near = c.z_near;
      float far = c.z_far;
      if (ImGui::DragFloat("Near Plane", &near, 0.1f)) {
        c.z_near = near;
      }
      if (ImGui::DragFloat("Far Plane", &far, 0.1f)) {
        c.z_far = far;
      }

      glm::vec3 color = c.clear_color;
      if (ImGui::ColorPicker3("Clear Color", &color.x)) {
        c.clear_color = color;
      }
    });

    draw_component<MeshRendererComponent>(
        "MeshRenderer", e, [](MeshRendererComponent &m) {
          ImGui::Text("Index count: %d", m.mesh.index_count);
        });

    // m_current_scene->script_engine().draw_components(m_current_scene->ecs(),
    // e);

    ImVec2 size = ImGui::CalcTextSize("Add Component");
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - size.x * 0.5f);
    if (ImGui::Button("Add Component")) {
      ImGui::OpenPopup("AddComponent");
    }
    if (ImGui::BeginPopup("AddComponent")) {
      draw_add_component<TransformComponent>("Transform", e);
      draw_add_component<CameraComponent>("Camera", e);
      draw_add_component<MeshRendererComponent>("MeshRenderer", e);
      // m_current_scene->script_engine().draw_add_component(
      // m_current_scene->ecs(), e);

      ImGui::EndPopup();
    }
  }

private:
  KEngine::Scene *m_current_scene;
  entt::entity m_selected_entity = entt::null;

  Project m_project;

  std::vector<entt::entity> m_entity_windows;

  std::string name;

  uint32_t m_fbo = 0;
  uint32_t m_fb_tex = 0, m_fb_depth_tex = 0;

  AssetBrowser m_asset_browser;
};

KEngine::App *KEngine::create_app() { return new EditorApp(); }

void colors() {
  ImGuiStyle &style = ImGui::GetStyle();
  style.FrameRounding = 2;
  style.ScrollbarRounding = 2;
  style.GrabRounding = 2;
  style.WindowPadding = ImVec2(4, 4);
  style.ScrollbarSize = 10;
  style.TabRounding = 2;
  style.TreeLinesFlags = ImGuiTreeNodeFlags_DrawLinesToNodes;
  style.TreeLinesSize = 1;
  style.TreeLinesRounding = 0;
  style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
  style.WindowMenuButtonPosition = ImGuiDir_None;
  style.DockingSeparatorSize = 2;

  ImVec4 *colors = style.Colors;

  constexpr const unsigned int dark = 0xff19181a;
  constexpr const unsigned int black = 0xff221f22;
  constexpr const unsigned int red = 0xffff6188;
  constexpr const unsigned int green = 0xffa9dc76;
  constexpr const unsigned int yellow = 0xffffd866;
  constexpr const unsigned int blue = 0xfffc9867;
  constexpr const unsigned int magenta = 0xffab9df2;
  constexpr const unsigned int cyan = 0xff78dce8;
  constexpr const unsigned int white = 0xfffcfcfa;
  constexpr const unsigned int dimmed1 = 0xffc1c0c0;
  constexpr const unsigned int dimmed2 = 0xff939293;
  constexpr const unsigned int dimmed3 = 0xff727072;
  constexpr const unsigned int dimmed4 = 0xff5b595c;
  constexpr const unsigned int dimmed5 = 0xff403e41;

  colors[ImGuiCol_Text] = ImColor(white);
  colors[ImGuiCol_TextDisabled] = ImColor(dimmed3);
  colors[ImGuiCol_TextLink] = ImColor(green);
  colors[ImGuiCol_WindowBg] = ImColor(dark);
  colors[ImGuiCol_ChildBg] = ImColor(dark);
  colors[ImGuiCol_PopupBg] = ImColor(dark);
  colors[ImGuiCol_Border] = ImColor(black);
  colors[ImGuiCol_BorderShadow] = ImColor(black);
  colors[ImGuiCol_FrameBg] = ImColor(black);
  colors[ImGuiCol_FrameBgHovered] = ImColor(dimmed2);
  colors[ImGuiCol_FrameBgActive] = ImColor(dimmed1);
  colors[ImGuiCol_TitleBg] = ImColor(black);
  colors[ImGuiCol_TitleBgActive] = ImColor(dark);
  colors[ImGuiCol_TitleBgCollapsed] = ImColor(dimmed4);
  colors[ImGuiCol_MenuBarBg] = ImColor(dark);
  colors[ImGuiCol_ScrollbarBg] = ImColor(dark);
  colors[ImGuiCol_ScrollbarGrab] = ImColor(dimmed3);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(dimmed2);
  colors[ImGuiCol_ScrollbarGrabActive] = ImColor(dimmed4);
  colors[ImGuiCol_CheckMark] = ImColor(green);
  colors[ImGuiCol_SliderGrab] = ImColor(dimmed3);
  colors[ImGuiCol_SliderGrabActive] = ImColor(dimmed2);
  colors[ImGuiCol_Button] = ImColor(dimmed3);
  colors[ImGuiCol_ButtonHovered] = ImColor(dimmed2);
  colors[ImGuiCol_ButtonActive] = ImColor(dimmed4);
  colors[ImGuiCol_Header] = ImColor(dimmed3);
  colors[ImGuiCol_HeaderHovered] = ImColor(dimmed2);
  colors[ImGuiCol_HeaderActive] = ImColor(dimmed4);
  colors[ImGuiCol_Separator] = ImColor(dimmed3);
  colors[ImGuiCol_SeparatorHovered] = ImColor(dimmed2);
  colors[ImGuiCol_SeparatorActive] = ImColor(dimmed4);
  colors[ImGuiCol_ResizeGrip] = ImColor(dimmed3);
  colors[ImGuiCol_ResizeGripHovered] = ImColor(dimmed4);
  colors[ImGuiCol_ResizeGripActive] = ImColor(dimmed2);
  colors[ImGuiCol_Tab] = ImColor(dimmed3);
  colors[ImGuiCol_TabHovered] = ImColor(dimmed4);
  colors[ImGuiCol_TabActive] = ImColor(dimmed2);
  colors[ImGuiCol_TabUnfocused] = ImColor(dimmed4);
  colors[ImGuiCol_TabUnfocusedActive] = ImColor(dimmed2);
  colors[ImGuiCol_TabSelectedOverline] = ImColor(dimmed4);
  colors[ImGuiCol_DockingPreview] = ImColor(dimmed3);
  colors[ImGuiCol_DockingEmptyBg] = ImColor(dark);
  colors[ImGuiCol_PlotLines] = ImColor(green);
  colors[ImGuiCol_PlotLinesHovered] = ImColor(green);
  colors[ImGuiCol_PlotHistogram] = ImColor(green);
  colors[ImGuiCol_PlotHistogramHovered] = ImColor(green);
  colors[ImGuiCol_TableHeaderBg] = ImColor(black);
  colors[ImGuiCol_TableBorderStrong] = ImColor(black);
  colors[ImGuiCol_TableBorderLight] = ImColor(white);
  colors[ImGuiCol_TableRowBg] = ImColor(black);
  colors[ImGuiCol_TableRowBgAlt] = ImColor(dark);
  colors[ImGuiCol_TextSelectedBg] = ImColor(dark);
  colors[ImGuiCol_DragDropTarget] = ImColor(green);
  colors[ImGuiCol_NavHighlight] = ImColor(green);
  colors[ImGuiCol_NavWindowingHighlight] = ImColor(green);
  colors[ImGuiCol_NavWindowingDimBg] = ImColor(dimmed2);
  colors[ImGuiCol_ModalWindowDimBg] = ImColor(dimmed2);
}
