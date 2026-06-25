/*
Copyright (c) テクニカル諏訪子

Permission is hereby granted to any person obtaining a copy of the software
Shader Playground (the "Software") to use, modify, merge, copy, publish, distribute,
sublicense, and/or sell copies of the Software, subject to the following conditions:

    1. **Origin Attribution**:
       - You must not misrepresent the origin of the Software; you must not claim
         you created the original Software.
       - If the Software is used in a product, you must either:
         a. Provide clear attribution in the product's documentation, user interface,
            or other visible areas, **OR**
         b. Pay the original developers a fee they specify in writing.
    2. **Usage Restriction**:
       - The Software, or any derivative works, dependencies, or libraries
         incorporating it, must not be used for censorship or to suppress freedom of
         speech, expression, or creativity. Prohibited uses include, but are not
         limited to:
         - Censorship of so-called "hate speech", visuals, non-mainstream opinions,
           ideas, or objective reality.
         - Tools or systems designed to restrict access to information or
           artistic works.
    3. **Notice Preservation**:
       - This license and the above copyright notice must remain intact in all copies
         of the source code.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <lua/luaengine.hh>
#include <glad/glad.h>
#include <shader/shader.hh>
#include <glfw/glfw3.h>
#include <gui/consolelog.hh>

namespace lua {
  LuaEngine::LuaEngine(const string &code, Program *prog, gui::ConsoleLog *cmd)
    : m_Code(code)
    , m_Program(prog)
    , m_Console(cmd)
  {
    m_Lua = MakeAPI();
  }

  LuaEngine::~LuaEngine() {}

  void LuaEngine::Reload(const string &code) {
    m_Code = code;
    Execute(m_Lua);
  }

  void LuaEngine::Update() {
    sol::function update = m_Lua["update"];
    if (update.valid()) {
      sol::protected_function_result res = update();

      if (!res.valid()) {
        sol::error err = res;
        LogError(err);
      }
    }
  }

  bool LuaEngine::Validate(const string &code) {
    try {
      sol::load_result resSyntax = m_Lua.load(code);
      if (!resSyntax.valid()) {
        sol::error err = resSyntax;
        LogError(err);
        return false;
      }

      sol::state testApi = MakeAPI();
      sol::protected_function_result resApi = testApi.safe_script(code);
      if (!resApi.valid()) {
        sol::error err = resApi;
        LogError(err);
        return false;
      }

      return true;
    } catch (const sol::error &err) {
      LogError(err);
      return false;
    }
  }

  ////////////////////

  void LuaEngine::LogError(sol::error err) {
    gui::LogEntry entry = {};
    entry.type = gui::LogType::Error;
    entry.text = err.what();
    if (m_Console) m_Console->Add(entry);
    else std::cerr << err.what() << std::endl;
  }

  void LuaEngine::LogError(const std::exception &err) {
    gui::LogEntry entry = {};
    entry.type = gui::LogType::Error;
    entry.text = err.what();
    if (m_Console) m_Console->Add(entry);
    else std::cerr << err.what() << std::endl;
  }

  sol::state LuaEngine::MakeAPI() {
    sol::state out;

    out.open_libraries(
      sol::lib::base,
      sol::lib::math,
      sol::lib::table,
      sol::lib::string
    );

    try {
      BindEngine(out);
      BindSystem(out);
      BindGraphics(out);
      Execute(out);
    } catch (const std::exception &e) {
      LogError(e);
      return {};
    }

    return out;
  }

  void LuaEngine::BindEngine(sol::state &lua) {
    lua["le"] = lua.create_table();
    lua["le"]["api_version"] = m_ApiVersion;
  }

  void LuaEngine::BindSystem(sol::state &lua) {
    lua["le"]["sys"] = lua.create_table();

    lua["le"]["sys"]["get_time"] = [&]() -> f32 {
      return (f32)glfwGetTime();
    };
  }

  void LuaEngine::BindGraphics(sol::state &lua) {
    lua["le"]["gfx"] = lua.create_table();

    lua["le"]["gfx"]["set_mesh"] = [&](sol::table t) {
      m_Mesh.vertices.clear();
      m_Mesh.indices.clear();
      m_Mesh.attr.clear();

      sol::table verts = t["vertices"];
      for (auto &v : verts) m_Mesh.vertices.push_back(v.second.as<f32>());

      sol::table inds = t["indices"];
      for (auto &i : inds) m_Mesh.indices.push_back(i.second.as<u32>());

      sol::table attrs = t["attributes"];
      for (auto &k : attrs) {
        sol::table a = k.second;

        sol::optional<int> location = a["location"];
        sol::optional<int> size = a["size"];
        sol::optional<int> stride = a["stride"];
        sol::optional<int> offset = a["offset"];

        if (!location || !size || !stride || !offset) {
          throw std::runtime_error("メッシュアトリビュートが不正です。");
        }

        m_Mesh.attr.push_back({
          a["location"],
          a["size"],
          a["stride"],
          a["offset"],
        });
      }
    };

    lua["le"]["gfx"]["set_uniform3"] = [&](const string &name, f32 x, f32 y, f32 z) {
      if (!m_Program) return;
      glUniform3f(m_Program->GetUniformLocation(name), x, y, z);
    };
  }

  void LuaEngine::Execute(sol::state &lua) {
    sol::protected_function_result res = lua.safe_script(m_Code);

    if (!res.valid()) {
      sol::error err = res;
      LogError(err);
    }
  }
} // namespace lua