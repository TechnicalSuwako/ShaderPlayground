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
//#include <utility>

namespace lua {
  LuaEngine::LuaEngine(const string &code) : m_Code(code) {
    m_Lua.open_libraries(
      sol::lib::base,
      sol::lib::math,
      sol::lib::table,
      sol::lib::string
    );

    BindGraphics();
    Execute();
  }

  LuaEngine::~LuaEngine() {
  }

  void LuaEngine::Reload(const string &code) {
    m_Code = code;
    //BindGraphics();
    Execute();
  }

  void LuaEngine::BindGraphics() {
    m_Lua["graphics"] = m_Lua.create_table();

    m_Lua["graphics"]["set_mesh"] = [&](sol::table t) {
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

        m_Mesh.attr.push_back({
          a["location"],
          a["size"],
          a["stride"],
          a["offset"],
        });
      }
    };
  }

  void LuaEngine::Execute() {
    sol::protected_function_result res = m_Lua.safe_script(m_Code);

    if (!res.valid()) {
      sol::error err = res;
      std::cerr << err.what() << std::endl;
    }
  }
} // namespace lua