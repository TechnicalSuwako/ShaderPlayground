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
#pragma once

#include <util/types.hh>
#include <util/structs.hh>
#define SOL_LUAJIT 1
#define SOL_ALL_SAFETIES_ON 1
#include <sol.hpp>

struct Program;

namespace gui {
  class ConsoleLog;
} // namespace gui

namespace lua {
  struct LuaMesh {
    vector<f32> vertices;
    vector<u32> indices;

    struct Attribute {
      int location;
      int size;
      int stride;
      int offset;
    };

    std::vector<Attribute> attr;

    GLsizei GetIndexCount() const {
      return static_cast<GLsizei>(indices.size());
    }
  };

  class LuaEngine {
    public:
      LuaEngine(const string &code, Program *prog, Info *info);
      ~LuaEngine();

    public:
      LuaMesh &GetMesh() { return m_Mesh; };
      const LuaMesh &GetMesh() const { return m_Mesh; };

    public:
      void Reload(const string &code);
      void Init();
      void Update();
      bool Validate(const string &code);

    public:
      void SetProgram(Program *prog) { m_Program = prog; }
      bool GetInitializationState() const { return m_IsInitialized; }
      void SetInitializationState(bool state) { m_IsInitialized = state; }
      string GetApiVersion() const { return m_ApiVersion; }

    private:
      void LogError(sol::error err);
      void LogError(const std::exception &err);
      void LogError(const string &err);
      sol::state MakeAPI();
      void BindEngine(sol::state &lua);
      void BindSystem(sol::state &lua);
      void BindIO(sol::state &lua);
      void BindMath(sol::state &lua);
      void BindGraphics(sol::state &lua);
      void Execute(sol::state &lua);

    private:
      LuaMesh m_Mesh;
      sol::state m_Lua;
      string m_Code;
      bool m_IsInitialized = false;
      Program *m_Program = nullptr;
      Info *m_Info = {};
      string m_ApiVersion = "1.0.0";
      f32 m_LastTime = 0.f;
  };
} // namespace lua