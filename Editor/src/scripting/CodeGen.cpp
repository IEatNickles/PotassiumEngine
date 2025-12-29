#include "scripting/CodeGen.hpp"

#include <cassert>
#include <cctype>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "Scene.hpp"

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

template <> struct std::formatter<std::vector<std::string_view>> {
  bool entt_hs = false;
  bool pid = false;

  template <class ParseContext>
  constexpr ParseContext::iterator parse(ParseContext &ctx) {
    auto it = ctx.begin();
    if (it == ctx.end()) {
      return it;
    }
    if (*it == 'h') {
      ++it;
      if (*it == 's') {
        entt_hs = true;
        ++it;
      } else {
        throw std::format_error("no hs?");
      }
    }
    if (*it == 'p') {
      ++it;
      if (*it == 'i') {
        ++it;
        if (*it == 'd') {
          pid = true;
          ++it;
        }
      } else {
        throw std::format_error("no hs?");
      }
    }
    if (it != ctx.end() && *it != '}') {
      throw std::format_error("no brace?");
    }
    return it;
  }

  template <class FmtContext>
  FmtContext::iterator format(std::vector<std::string_view> const &v,
                              FmtContext &ctx) const {
    std::ostringstream out;
    int i = 0;
    for (auto item : v) {
      if (entt_hs) {
        out << "\"" << item << "\"_hs";
      } else if (pid) {
        out << "{prop_ids[" << i << "], \"" << item << "\"}";
      } else {
        out << item;
      }
      if (i < v.size() - 1) {
        out << ", ";
      }
      ++i;
    }
    return std::ranges::copy(std::move(out).str(), ctx.out()).out;
  }
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

        std::string ps = path.string();
        os << std::vformat(
            R"(
#include <vector>
#include <cstdint>

#include "nlohmann/json.hpp"
#include "entt/entt.hpp"

#include "{0}"

using namespace entt::literals;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE({1}, {2});

namespace {1}Reflected {{
inline std::vector<entt::id_type> prop_ids = {{ {2:hs} }};

inline std::map<entt::id_type, char const *> id_to_name = {{
  {{entt::type_id<{1}>().index(), "{1}"}},
  {2:pid}
}};

inline void save(nlohmann::json &j, entt::registry const &reg) {{
  nlohmann::json cs = nlohmann::json::array();
  reg.view<{1}>().each([&cs](entt::entity e, {1} const &c) {{
    cs.push_back({{ {{ "entity", (uint64_t)e }}, {{ "data", c }} }});
  }});
  j["{1}"] = cs;
}}

inline void load(nlohmann::json const &j, entt::registry &reg) {{
  for (auto val : j) {{
    entt::entity e = (entt::entity)val["entity"].get<uint64_t>();
    {1} c = j["data"].get<{1}>();
    reg.emplace<{1}>(e, c);
  }}
}}
}} // namespace {1}Reflected
)",
            std::make_format_args(ps, c.name, c.prop_names));

        os << "#include <vector>\n"
              "#include <cstdint>\n\n"
              "#include \"nlohmann/json.hpp\"\n"
              "#include \"entt/entt.hpp\"\n\n"
              "using namespace entt::literals;\n\n"
              "NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE("
           << c.name;
        for (auto p : c.prop_names) {
          os << ", " << p;
        }

        os << "\n\n";
        os << "namespace " << c.name << "Reflected {\n";
        os << "inline std::vector<entt::id_type> prop_ids = {\n";
        for (auto p : c.prop_names) {
          os << "\"" << p << "\"_hs,\n";
        }
        os << "}\n";

        os << "inline std::map<entt::id_type, char const *> id_to_name = {\n"
              "  {entt::type_id<Foo>().index(), \"Foo\"},\n";
        int i = 0;
        for (auto p : c.prop_names) {
          os << std::vformat("  {{prop_ids[{0}], \"{1}\"},\n",
                             std::make_format_args(i, p));
          ++i;
        }
        os << "};\n";
        os << "}\n";

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
             "#include <cstdint>\n\n"
             "#include \"nlohmann/json.hpp\"\n"
             "#include \"entt/entt.hpp\"\n"
             "#include \"imgui.h\"\n\n";

  std::vector<Component> components;
  components.reserve(scripts.size());
  for (auto script : scripts) {
    std::ifstream ifs(script);
    std::stringstream ss;
    ss << ifs.rdbuf();

    auto filename = script.filename().string() + "-generated.hpp";
    auto path = script.parent_path() / filename;
    std::filesystem::create_directories("./generated" / path.parent_path());
    std::ofstream ofs("generated" / path);
    generate_script(script, ofs, ss.view(), components);
    ofs.close();
    ifs.close();

    init_fs << "#include " << path << "\n";
  }
  init_fs << "\n";

  // save
  init_fs << "extern \"C\" void save(nlohmann::json &j, entt::registry const "
             "&reg) {";

  for (auto c : components) {
    init_fs << "\n  " << c.name << "Reflected::save(j, reg);";
  }

  init_fs << "\n}\n";
  // -----------

  // load
  init_fs << "extern \"C\" void load(nlohmann::json const &j, entt::registry "
             "&reg) {\n";
  for (auto c : components) {
    init_fs << std::vformat("    if (j.contains(\"{0}\")) {{\n"
                            "      {0}Reflected::load(j[\"{0}\"], reg);\n"
                            "    }}\n",
                            std::make_format_args(c.name));
  }
  init_fs << "\n  }\n}\n";
  // -----------

  // add_component
  init_fs << "extern \"C\" void add_component(entt::registry &reg, std::string "
             "const &name, entt::entity e) {\n";
  for (auto c : components) {
    init_fs << "   if (name == \"" << c.name << "\") {\n";
    init_fs << "     reg.emplace<" << c.name << ">(e);\n";
    init_fs << "     return;\n";
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
    init_fs << "    if (ImGui::MenuItem(\"" << c.name << "\")) {\n";
    init_fs << "      reg.emplace<" << c.name << ">(e);\n";
    init_fs << "    }\n";
    init_fs << "  }\n";
  }
  init_fs << "}\n";
  // ------------------

  init_fs.close();
}
} // namespace KEditor
