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

void error_callback(int error_code, char const *description) {
  std::cout << "GLFW Error: " << error_code << ": " << description << "\n";
}

class EditorApp : public KEngine::App {
public:
  virtual void run() override {
    m_current_scene = new KEngine::Scene();

    glfwSetErrorCallback(error_callback);

    glfwInit();
    GLFWwindow *window =
        glfwCreateWindow(1280, 720, "Potassium", nullptr, nullptr);
    glfwMaximizeWindow(window);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    io.Fonts->AddFontFromFileTTF(
        "Editor/assets/fonts/JetBrainsMono/JetBrainsMono-Regular.ttf", 16);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glGenTextures(1, &m_fb_tex);
    glBindTexture(GL_TEXTURE_2D, m_fb_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           m_fb_tex, 0);

    glGenTextures(1, &m_fb_depth_tex);
    glBindTexture(GL_TEXTURE_2D, m_fb_depth_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, GL_TEXTURE_2D,
                           m_fb_depth_tex, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      DEBUG_ERROR("Failed to make framebuffer, {}", glGetError());
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      ImGui::DockSpaceOverViewport();

      if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
          if (ImGui::MenuItem("Save")) {
            char *file =
                tinyfd_saveFileDialog("Save File", "~/", 0, nullptr, nullptr);
            if (file != nullptr) {
              DEBUG_TRACE("saving file: {}", file);
              m_current_scene->save(file);
            }
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
          if (ImGui::MenuItem("Path?")) {
            DEBUG_TRACE("{}", std::filesystem::current_path().c_str());
          }
          if (ImGui::MenuItem("Compile Scripts")) {
            KEditor::CodeGen cg;
            std::vector<std::filesystem::path> scripts = {};
            for (auto f : std::filesystem::recursive_directory_iterator(
                     "./Sandbox/assets")) {
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

      ImGui::Begin("Scene");
      if (ImGui::Button("Add Entity")) {
        m_selected_entity = m_current_scene->entity();
      }

      if (m_current_scene) {
        auto flags = ImGuiTreeNodeFlags_DefaultOpen |
                     ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_Framed;
        if (ImGui::TreeNodeEx(m_current_scene->name().c_str(), flags)) {
          m_current_scene->ecs().view<entt::entity>().each(
              [&](entt::entity e) { draw_entity(e); });
          ImGui::TreePop();
        }
      }

      ImGui::End(); // Scene

      ImGui::Begin("Viewport");
      glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
      glViewport(0, 0, 1920, 1080);
      m_current_scene->update();
      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      ImGui::Image((ImTextureID)(intptr_t)m_fb_tex, ImVec2(1280, 720));
      ImGui::End(); // Viewport

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
  }

  template <typename T, typename Fn>
  void draw_component(std::string const &name, entt::entity e, Fn draw_fn) {
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
    if (ImGui::Button(name.c_str())) {
      m_current_scene->set<T>(e);
      ImGui::CloseCurrentPopup();
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
          ImGui::Text("Yup, it's a mesh renderer alright");
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

  std::vector<entt::entity> m_entity_windows;

  std::string name;

  uint32_t m_fbo;
  uint32_t m_fb_tex, m_fb_depth_tex;
};

KEngine::App *KEngine::create_app() { return new EditorApp(); }
