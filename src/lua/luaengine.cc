/*
# 076 License

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
#include <glad/glad.h>
#include <lua/luaengine.hh>
#include <shader/shader.hh>
#include <glfw/glfw3.h>
#include <util/glfwpp.hh>
#include <gui/consolelog.hh>
#include <util/vector.hh>
#include <util/random.hh>

namespace lua {
  LuaEngine::LuaEngine(const string &code, Program *prog, GlfwInfo *glfwInfo, gui::ConsoleLog *cmd)
    : m_Code(code)
    , m_Program(prog)
    , m_GlfwInfo(glfwInfo)
    , m_Console(cmd)
  {
    m_Lua = MakeAPI();
    Execute(m_Lua);
  }

  LuaEngine::~LuaEngine() {}

  void LuaEngine::Reload(const string &code) {
    m_Code = code;
    m_Lua = MakeAPI();
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
      sol::load_result resApi = m_Lua.load(code);
      //sol::protected_function_result resApi = testApi.safe_script(code);
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

    out.new_usertype<Vector2i>(
      "Vector2i",
      "x", sol::property(
        [](Vector2i &v) { return v.pos.x; },
        [](Vector2i &v, i32 val) { v.pos.x = val; }
      ),
      "y", sol::property(
        [](Vector2i &v) { return v.pos.y; },
        [](Vector2i &v, i32 val) { v.pos.y = val; }
      )
    );

    out.new_usertype<Vector2u>(
      "Vector2u",
      "x", sol::property(
        [](Vector2u &v) { return v.pos.x; },
        [](Vector2u &v, u32 val) { v.pos.x = val; }
      ),
      "y", sol::property(
        [](Vector2u &v) { return v.pos.y; },
        [](Vector2u &v, u32 val) { v.pos.y = val; }
      )
    );

    out.new_usertype<Vector2f>(
      "Vector2f",
      "x", sol::property(
        [](Vector2f &v) { return v.pos.x; },
        [](Vector2f &v, f32 val) { v.pos.x = val; }
      ),
      "y", sol::property(
        [](Vector2f &v) { return v.pos.y; },
        [](Vector2f &v, f32 val) { v.pos.y = val; }
      )
    );

    out.new_usertype<Vector2d>(
      "Vector2d",
      "x", sol::property(
        [](Vector2d &v) { return v.pos.x; },
        [](Vector2d &v, i32 val) { v.pos.x = val; }
      ),
      "y", sol::property(
        [](Vector2d &v) { return v.pos.y; },
        [](Vector2d &v, f64 val) { v.pos.y = val; }
      )
    );

    out.new_usertype<Vector3i>(
      "Vector3i",
      "x", sol::property(
        [](Vector3i &v) { return v.pos.x; },
        [](Vector3i &v, i32 val) { v.pos.x = val; }
      ),
      "y", sol::property(
        [](Vector3i &v) { return v.pos.y; },
        [](Vector3i &v, i32 val) { v.pos.y = val; }
      ),
      "z", sol::property(
        [](Vector3i &v) { return v.pos.z; },
        [](Vector3i &v, i32 val) { v.pos.z = val; }
      )
    );

    out.new_usertype<Vector3u>(
      "Vector3u",
      "x", sol::property(
        [](Vector3u &v) { return v.pos.x; },
        [](Vector3u &v, u32 val) { v.pos.x = val; }
      ),
      "y", sol::property(
        [](Vector3u &v) { return v.pos.y; },
        [](Vector3u &v, u32 val) { v.pos.y = val; }
      ),
      "z", sol::property(
        [](Vector3u &v) { return v.pos.z; },
        [](Vector3u &v, u32 val) { v.pos.z = val; }
      )
    );

    out.new_usertype<Vector3f>(
      "Vector3f",
      "x", sol::property(
        [](Vector3f &v) { return v.pos.x; },
        [](Vector3f &v, f32 val) { v.pos.x = val; }
      ),
      "y", sol::property(
        [](Vector3f &v) { return v.pos.y; },
        [](Vector3f &v, f32 val) { v.pos.y = val; }
      ),
      "z", sol::property(
        [](Vector3f &v) { return v.pos.z; },
        [](Vector3f &v, f32 val) { v.pos.z = val; }
      )
    );

    out.new_usertype<Vector3d>(
      "Vector3d",
      "x", sol::property(
        [](Vector3d &v) { return v.pos.x; },
        [](Vector3d &v, i32 val) { v.pos.x = val; }
      ),
      "y", sol::property(
        [](Vector3d &v) { return v.pos.y; },
        [](Vector3d &v, f64 val) { v.pos.y = val; }
      ),
      "z", sol::property(
        [](Vector3d &v) { return v.pos.z; },
        [](Vector3d &v, f64 val) { v.pos.z = val; }
      )
    );

    out.new_usertype<Vector4i>(
      "Vector4i",
      "x", sol::property(
        [](Vector4i &v) { return v.pos.x; },
        [](Vector4i &v, i32 val) { v.pos.x = val; }
      ),
      "y", sol::property(
        [](Vector4i &v) { return v.pos.y; },
        [](Vector4i &v, i32 val) { v.pos.y = val; }
      ),
      "z", sol::property(
        [](Vector4i &v) { return v.pos.z; },
        [](Vector4i &v, i32 val) { v.pos.z = val; }
      ),
      "w", sol::property(
        [](Vector4i &v) { return v.pos.w; },
        [](Vector4i &v, i32 val) { v.pos.w = val; }
      )
    );

    out.new_usertype<Vector4u>(
      "Vector4u",
      "x", sol::property(
        [](Vector4u &v) { return v.pos.x; },
        [](Vector4u &v, u32 val) { v.pos.x = val; }
      ),
      "y", sol::property(
        [](Vector4u &v) { return v.pos.y; },
        [](Vector4u &v, u32 val) { v.pos.y = val; }
      ),
      "z", sol::property(
        [](Vector4u &v) { return v.pos.z; },
        [](Vector4u &v, u32 val) { v.pos.z = val; }
      ),
      "w", sol::property(
        [](Vector4u &v) { return v.pos.w; },
        [](Vector4u &v, u32 val) { v.pos.w = val; }
      )
    );

    out.new_usertype<Vector4f>(
      "Vector4f",
      "x", sol::property(
        [](Vector4f &v) { return v.pos.x; },
        [](Vector4f &v, f32 val) { v.pos.x = val; }
      ),
      "y", sol::property(
        [](Vector4f &v) { return v.pos.y; },
        [](Vector4f &v, f32 val) { v.pos.y = val; }
      ),
      "z", sol::property(
        [](Vector4f &v) { return v.pos.z; },
        [](Vector4f &v, f32 val) { v.pos.z = val; }
      ),
      "w", sol::property(
        [](Vector4f &v) { return v.pos.w; },
        [](Vector4f &v, f32 val) { v.pos.w = val; }
      )
    );

    out.new_usertype<Vector4d>(
      "Vector4d",
      "x", sol::property(
        [](Vector4d &v) { return v.pos.x; },
        [](Vector4d &v, i32 val) { v.pos.x = val; }
      ),
      "y", sol::property(
        [](Vector4d &v) { return v.pos.y; },
        [](Vector4d &v, f64 val) { v.pos.y = val; }
      ),
      "z", sol::property(
        [](Vector4d &v) { return v.pos.z; },
        [](Vector4d &v, f64 val) { v.pos.z = val; }
      ),
      "w", sol::property(
        [](Vector4d &v) { return v.pos.w; },
        [](Vector4d &v, f64 val) { v.pos.w = val; }
      )
    );

    try {
      BindEngine(out);
      BindSystem(out);
      BindIO(out);
      BindGraphics(out);
    } catch (const std::exception &e) {
      LogError(e);
      return {};
    }

    return out;
  }

  void LuaEngine::BindEngine(sol::state &lua) {
    lua["le"] = lua.create_table();
    lua["le"]["api_version"] = m_ApiVersion;

    lua["le"]["log"] = [&](sol::variadic_args va) {
      string out;

      for (auto v : va) {
        if (v.is<string>())
          out += v.as<string>();
        else if (v.is<i32>())
          out += std::to_string(v.as<i32>());
        else if (v.is<f64>())
          out += std::to_string(v.as<f64>());
        else if (v.is<bool>())
          out += v.as<bool>() ? "true" : "false";
        else if (v.is<Vector2i>())
          out += "{" + std::to_string(v.as<Vector2i>().pos.x) + ", " + std::to_string(v.as<Vector2i>().pos.y) + "}";
        else if (v.is<Vector2u>())
          out += "{" + std::to_string(v.as<Vector2u>().pos.x) + ", " + std::to_string(v.as<Vector2u>().pos.y) + "}";
        else if (v.is<Vector2f>())
          out += "{" + std::to_string(v.as<Vector2f>().pos.x) + ", " + std::to_string(v.as<Vector2f>().pos.y) + "}";
        else if (v.is<Vector2d>())
          out += "{" + std::to_string(v.as<Vector2d>().pos.x) + ", " + std::to_string(v.as<Vector2d>().pos.y) + "}";
        else if (v.is<Vector3i>())
          out += "{" + std::to_string(v.as<Vector3i>().pos.x) + ", " + std::to_string(v.as<Vector3i>().pos.y) + ", " + std::to_string(v.as<Vector3i>().pos.z) + "}";
        else if (v.is<Vector3u>())
          out += "{" + std::to_string(v.as<Vector3u>().pos.x) + ", " + std::to_string(v.as<Vector3u>().pos.y) + ", " + std::to_string(v.as<Vector3u>().pos.z) + "}";
        else if (v.is<Vector3f>())
          out += "{" + std::to_string(v.as<Vector3f>().pos.x) + ", " + std::to_string(v.as<Vector3f>().pos.y) + ", " + std::to_string(v.as<Vector3f>().pos.z) + "}";
        else if (v.is<Vector3d>())
          out += "{" + std::to_string(v.as<Vector3d>().pos.x) + ", " + std::to_string(v.as<Vector3d>().pos.y) + ", " + std::to_string(v.as<Vector3d>().pos.z) + "}";
        else if (v.is<Vector4i>())
          out += "{" + std::to_string(v.as<Vector4i>().pos.x) + ", " + std::to_string(v.as<Vector4i>().pos.y) + ", " + std::to_string(v.as<Vector4i>().pos.z) + ", " + std::to_string(v.as<Vector4i>().pos.w) + "}";
        else if (v.is<Vector4u>())
          out += "{" + std::to_string(v.as<Vector4u>().pos.x) + ", " + std::to_string(v.as<Vector4u>().pos.y) + ", " + std::to_string(v.as<Vector4u>().pos.z) + ", " + std::to_string(v.as<Vector4u>().pos.w) + "}";
        else if (v.is<Vector4f>())
          out += "{" + std::to_string(v.as<Vector4f>().pos.x) + ", " + std::to_string(v.as<Vector4f>().pos.y) + ", " + std::to_string(v.as<Vector4f>().pos.z) + ", " + std::to_string(v.as<Vector4f>().pos.w) + "}";
        else if (v.is<Vector4d>())
          out += "{" + std::to_string(v.as<Vector4d>().pos.x) + ", " + std::to_string(v.as<Vector4d>().pos.y) + ", " + std::to_string(v.as<Vector4d>().pos.z) + ", " + std::to_string(v.as<Vector4d>().pos.w) + "}";
        else if (v.get_type() == sol::type::table) {
          sol::table t = v.as<sol::table>();
          out += "{";

          bool first = true;
          for (auto &kv : t) {
            if (!first) out += ", ";
            first = false;

            if (kv.second.is<i32>())
              out += std::to_string(kv.second.as<i32>());
            else if (kv.second.is<f64>())
              out += std::to_string(kv.second.as<f64>());
            else if (kv.second.is<string>())
              out += kv.second.as<string>();
            else
              out += "undefined";
          }

          out += "}";
        }
        else
          out += "undefined";

        out += " ";
      }

      gui::LogEntry entry = {};
      entry.type = gui::LogType::Normal;
      entry.text = out;
      if (m_Console) m_Console->Add(entry);
      else std::cerr << entry.text << std::endl;
    };
  }

  void LuaEngine::BindSystem(sol::state &lua) {
    lua["le"]["sys"] = lua.create_table();

    lua["le"]["sys"]["get_time"] = [&]() -> f32 {
      return (f32)m_GlfwInfo->instance->GetTime();
    };

    lua["le"]["sys"]["get_delta_time"] = [&]() -> f32 {
      f32 now = (f32)m_GlfwInfo->instance->GetTime();
      f32 delta = now - m_LastTime;
      m_LastTime = now;
      if (delta > 1.f / 5) delta = 1.f / 5;
      return delta;
    };

    lua["le"]["sys"]["get_timer_frequency"] = [&]() -> u64 {
      return m_GlfwInfo->instance->GetTimerFrequency();
    };

    lua["le"]["sys"]["get_resolution"] = [&]() -> Vector2i {
      Vector2i size = {};
      m_GlfwInfo->window->GetSize(&size.pos.x, &size.pos.y);
      return size;
    };

    lua["le"]["sys"]["random"] = [&](i32 min, i32 max) -> i32 {
      return util::rand::GetRandom(min, max);
    };
  }

  void LuaEngine::BindIO(sol::state &lua) {
    lua["le"]["io"] = lua.create_table();

    lua["le"]["io"]["get_mouse"] = [&]() -> Vector2d {
      Vector2d cursor{};
      m_GlfwInfo->window->GetCursorPos(&cursor.pos.x, &cursor.pos.y);
      return cursor;
    };

    lua["le"]["io"]["is_key_down"] = [&](int key) -> bool {
      return m_GlfwInfo->window->GetKey(key) == GLFW_PRESS;
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

    lua["le"]["gfx"]["set_uniform1"] = [&](const string &name, f32 x) {
      if (!m_Program) return;
      glUniform1f(m_Program->GetUniformLocation(name), x);
    };

    lua["le"]["gfx"]["set_uniform2"] = [&](const string &name, f32 x, f32 y) {
      if (!m_Program) return;
      glUniform2f(m_Program->GetUniformLocation(name), x, y);
    };

    lua["le"]["gfx"]["set_uniform3"] = [&](const string &name, f32 x, f32 y, f32 z) {
      if (!m_Program) return;
      glUniform3f(m_Program->GetUniformLocation(name), x, y, z);
    };

    lua["le"]["gfx"]["set_uniform4"] = [&](const string &name, f32 x, f32 y, f32 z, f32 w) {
      if (!m_Program) return;
      glUniform4f(m_Program->GetUniformLocation(name), x, y, z, w);
    };

    lua["le"]["gfx"]["set_uniform_float1"] = [&](const string &name, f32 x) {
      if (!m_Program) return;
      glUniform1f(m_Program->GetUniformLocation(name), x);
    };

    lua["le"]["gfx"]["set_uniform_float2"] = [&](const string &name, f32 x, f32 y) {
      if (!m_Program) return;
      glUniform2f(m_Program->GetUniformLocation(name), x, y);
    };

    lua["le"]["gfx"]["set_uniform_float3"] = [&](const string &name, f32 x, f32 y, f32 z) {
      if (!m_Program) return;
      glUniform3f(m_Program->GetUniformLocation(name), x, y, z);
    };

    lua["le"]["gfx"]["set_uniform_float4"] = [&](const string &name, f32 x, f32 y, f32 z, f32 w) {
      if (!m_Program) return;
      glUniform4f(m_Program->GetUniformLocation(name), x, y, z, w);
    };

    lua["le"]["gfx"]["set_uniform_double1"] = [&](const string &name, f64 x) {
      if (!m_Program) return;
      glUniform1d(m_Program->GetUniformLocation(name), x);
    };

    lua["le"]["gfx"]["set_uniform_double2"] = [&](const string &name, f64 x, f64 y) {
      if (!m_Program) return;
      glUniform2d(m_Program->GetUniformLocation(name), x, y);
    };

    lua["le"]["gfx"]["set_uniform_double3"] = [&](const string &name, f64 x, f64 y, f64 z) {
      if (!m_Program) return;
      glUniform3d(m_Program->GetUniformLocation(name), x, y, z);
    };

    lua["le"]["gfx"]["set_uniform_double4"] = [&](const string &name, f64 x, f64 y, f64 z, f64 w) {
      if (!m_Program) return;
      glUniform4d(m_Program->GetUniformLocation(name), x, y, z, w);
    };

    lua["le"]["gfx"]["set_uniform_int1"] = [&](const string &name, i32 x) {
      if (!m_Program) return;
      glUniform1i(m_Program->GetUniformLocation(name), x);
    };

    lua["le"]["gfx"]["set_uniform_int2"] = [&](const string &name, i32 x, i32 y) {
      if (!m_Program) return;
      glUniform2i(m_Program->GetUniformLocation(name), x, y);
    };

    lua["le"]["gfx"]["set_uniform_int3"] = [&](const string &name, i32 x, i32 y, i32 z) {
      if (!m_Program) return;
      glUniform3i(m_Program->GetUniformLocation(name), x, y, z);
    };

    lua["le"]["gfx"]["set_uniform_int4"] = [&](const string &name, i32 x, i32 y, i32 z, i32 w) {
      if (!m_Program) return;
      glUniform4i(m_Program->GetUniformLocation(name), x, y, z, w);
    };

    lua["le"]["gfx"]["set_uniform_uint1"] = [&](const string &name, u32 x) {
      if (!m_Program) return;
      glUniform1ui(m_Program->GetUniformLocation(name), x);
    };

    lua["le"]["gfx"]["set_uniform_uint2"] = [&](const string &name, u32 x, u32 y) {
      if (!m_Program) return;
      glUniform2ui(m_Program->GetUniformLocation(name), x, y);
    };

    lua["le"]["gfx"]["set_uniform_uint3"] = [&](const string &name, u32 x, u32 y, u32 z) {
      if (!m_Program) return;
      glUniform3ui(m_Program->GetUniformLocation(name), x, y, z);
    };

    lua["le"]["gfx"]["set_uniform_uint4"] = [&](const string &name, u32 x, u32 y, u32 z, u32 w) {
      if (!m_Program) return;
      glUniform4ui(m_Program->GetUniformLocation(name), x, y, z, w);
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