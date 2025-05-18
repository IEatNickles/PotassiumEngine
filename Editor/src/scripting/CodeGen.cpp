#include "scripting/CodeGen.hpp"
#include <cassert>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

enum CppTokenType {
  CPP_INVALID,
  CPP_EOF,
  CPP_IDENT,
  CPP_SEMICOLON,
  CPP_POUND,
  CPP_DQUOTE,
  CPP_SQUOTE,

  CPP_OPEN_PAREN,
  CPP_CLOSE_PAREN,
  CPP_OPEN_BRACE,
  CPP_CLOSE_BRACE,
};

struct CppToken {
  CppTokenType type;
  std::string_view text;
};

class CppLexer {
public:
  CppLexer() : m_idx(0) {}
  CppLexer(std::string_view const &src) : m_src(src), m_idx(0) {}

  CppToken peek_token() {
    size_t start = m_idx;
    CppToken t = next_token();
    m_idx = start;
    return t;
  }

  CppToken next_token() {
    char c = next();
    if (c == 0) {
      return CppToken{CPP_EOF};
    }
    while (std::isspace(c)) {
      c = next();
    }

    switch (c) {
    case ';':
      return CppToken{CPP_SEMICOLON, m_src.substr(m_idx - 1, 1)};
    case '"':
      return CppToken{CPP_DQUOTE, m_src.substr(m_idx - 1, 1)};
    case '\'':
      return CppToken{CPP_SQUOTE, m_src.substr(m_idx - 1, 1)};
    case '(':
      return CppToken{CPP_OPEN_PAREN, m_src.substr(m_idx - 1, 1)};
    case ')':
      return CppToken{CPP_CLOSE_PAREN, m_src.substr(m_idx - 1, 1)};
    case '{':
      return CppToken{CPP_OPEN_BRACE, m_src.substr(m_idx - 1, 1)};
    case '}':
      return CppToken{CPP_CLOSE_BRACE, m_src.substr(m_idx - 1, 1)};
    case '#':
      return CppToken{CPP_POUND, m_src.substr(m_idx - 1, 1)};
    }

    if (std::isalpha(c)) {
      size_t start = m_idx - 1;
      while (std::isalnum(c)) {
        c = next();
      }
      --m_idx;
      size_t len = m_idx - start;

      return CppToken{CPP_IDENT, m_src.substr(start, len)};
    }

    return CppToken{CPP_INVALID, std::to_string(m_idx)};
  }

private:
  char next() {
    if (m_idx >= m_src.size())
      return 0;
    return m_src[m_idx++];
  }

private:
  size_t m_idx;
  std::string_view m_src;
};

namespace KEditor {
struct Component {
  std::string_view name;
  std::vector<std::string_view> prop_names;
};

void generate_script(std::filesystem::path const &path, std::ofstream &os,
                     std::string_view const &source,
                     std::vector<Component> &component_names) {
  CppLexer lexer(source);
  for (;;) {
    CppToken t = lexer.next_token();
    if (t.type == CPP_EOF)
      break;

    if (t.type == CPP_IDENT) {
      if (t.text == "KCOMPONENT") {
        lexer.next_token(); // (
        lexer.next_token(); // )
        lexer.next_token(); // struct or class
        CppToken component_name_token = lexer.next_token();
        assert(component_name_token.type == CPP_IDENT);
        Component c;
        c.name = component_name_token.text;
        if (lexer.peek_token().type == CPP_OPEN_BRACE) {
          lexer.next_token();
          while (lexer.peek_token().type != CPP_CLOSE_BRACE) {
            assert(lexer.next_token().type == CPP_IDENT);
            CppToken ident = lexer.next_token();
            assert(lexer.next_token().type == CPP_SEMICOLON);

            c.prop_names.push_back(ident.text);
          }
        }

        os << "#include <vector>\n"
              "#include <nlohman/json.h>\n"
              "#include <cstdint>\n"
              "#include <flecs.h>\n"
              "using namespace entt::literals;\n\n";

        os << "#include " << path << "\n\n";

        os << "namespace " << c.name << "Reflected {\n";
        os << "inline std::vector<entt::id_type> prop_ids = {\n";
        for (auto p : c.prop_names) {
          os << "  \"" << p << "\"_hs,\n";
        }
        os << "};\n";
        os << "inline std::map<entt::id_type, const char *> id_to_name = {\n";

        os << "  {entt::type_id<" << c.name << ">().index(), \"" << c.name
           << "\"},\n";
        int i = 0;
        for (auto p : c.prop_names) {
          os << "  {prop_ids[" << i << "], \"" << p << "\"},\n";
          ++i;
        }
        os << "};\n\n";

        os << "inline void save(nlohman::json &j, flecs::entity e) {\n";
        os << "nlohman::json node = j.object();";
        os << "  e.get<" << c.name << ">([&j](" << c.name << " const &c) {";
        for (auto p : c.prop_names) {
          os << "\n  node[\"" << p << "\"] = c." << p << ";";
        }
        os << "  });\n";
        os << "  j[\"" << c.name << "\"] = node;";
        os << "}\n\n";

        os << "inline void load(nlohman::json const &j, entt::registry &reg, "
              "entt::entity e) {\n  reg.emplace<"
           << c.name << ">(e";
        for (auto p : c.prop_names) {
          os << ",\n    node[\"" << p << "\"].as<decltype(" << c.name
             << "::" << p << ")>()";
        }
        os << ");\n";
        os << "}\n";
        os << "} // namespace " << c.name << "Reflected\n";

        component_names.push_back(c);
      } else if (t.text == "KSYSTEM") {
      }
    }
  }
}

void CodeGen::generate(std::vector<std::filesystem::path> const &scripts) {
  std::filesystem::create_directory("./generated");
  std::ofstream init_fs("./generated/init.cpp");
  init_fs << "#include <string>\n"
             "#include <type_traits>\n"
             "#include <yaml-cpp/yaml.h>\n\n"
             "#include <entt/entt.hpp>\n"
             "#include <imgui.h>\n";

  std::vector<Component> components;
  components.reserve(scripts.size());
  for (auto script : scripts) {
    std::ifstream ifs(script);
    std::stringstream ss;
    ss << ifs.rdbuf();

    auto path = script.parent_path() /
                script.filename().stem().string().append("-generated.hpp");
    std::filesystem::create_directories("./generated" / path.parent_path());
    std::ofstream ofs("generated" / path);
    generate_script(script, ofs, ss.view(), components);
    ofs.close();
    ifs.close();

    init_fs << "#include " << path << "\n";
  }
  init_fs << "\n";

  // save
  init_fs << "extern \"C\" void save(YAML::Emitter &out, entt::registry const "
             "&reg) {"
             "  out << YAML::Key << \"Entities\";";

  for (auto c : components) {
    init_fs << "\n  " << c.name << "Reflected::save(out, reg);";
  }

  init_fs << "\n}\n";
  // -----------

  // load
  init_fs << "extern \"C\" void load(YAML::Node &node, entt::registry &reg) {\n"
             "  int i = 0;\n"
             "  for (auto e : node) {\n";
  for (auto c : components) {
    init_fs << "    if (e[\"Foo\"].IsDefined())\n";
    init_fs << "      " << c.name
            << "Reflected::load(node, reg, (entt::entity)i);\n";
  }
  init_fs << "    ++i;\n  }\n}\n";
  // -----------

  // add_component
  init_fs << "extern \"C\" void add_component(entt::registry &reg, std::string "
             "name,\n";
  init_fs << "                               entt::entity e) {\n";
  for (auto c : components) {
    init_fs << "   if (name == \"" << c.name << "\") {\n";
    init_fs << "     reg.emplace<" << c.name
            << ">(e);\n"
               "     return;\n";
    init_fs << "   }\n";
  }
  init_fs << " }\n";
  // --------------

  // draw_components
  init_fs << "extern \"C\" void draw_components(entt::registry &reg, "
             "entt::entity e) {\n";
  for (auto c : components) {
    init_fs << "  if (reg.all_of<" << c.name << ">(e)) {\n";
    init_fs << "    if (ImGui::CollapsingHeader(\"" << c.name << "\")) {\n";
    init_fs << "      auto &c = reg.get<" << c.name << ">(e);\n";
    for (auto p : c.prop_names) {
      init_fs << "      if constexpr (std::is_same<decltype(" << c.name
              << "::" << p << "), int>()) {\n";
      init_fs << "        ImGui::DragInt(\"" << p << "\", (int*)&c." << p
              << ");\n";
      init_fs << "      } ";
      init_fs << "else if constexpr (std::is_same<decltype(" << c.name
              << "::" << p << "), float>()) {\n";
      init_fs << "        ImGui::DragFloat(\"" << p << "\", (float*)&c." << p
              << ");\n";
      init_fs << "      }\n";
    }
    init_fs << "    }\n";
    init_fs << "  }\n";
  }
  init_fs << "}\n";
  // ---------------

  // draw_add_component
  init_fs << "extern \"C\" void draw_add_component(entt::registry &reg, "
             "entt::entity e) {\n";
  for (auto c : components) {
    init_fs << "  if (!reg.all_of<" << c.name << ">(e)) {\n";
    init_fs << "    if (ImGui::Button(\"" << c.name << "\")) {\n";
    init_fs << "      reg.emplace<" << c.name << ">(e);\n";
    init_fs << "      ImGui::CloseCurrentPopup();\n";
    init_fs << "    }\n";
    init_fs << "  }\n";
  }
  init_fs << "}\n";
  // ------------------

  init_fs.close();
}
} // namespace KEditor
