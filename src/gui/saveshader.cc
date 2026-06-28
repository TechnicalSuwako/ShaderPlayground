/*
# 076 License

Copyright (c) 076 Studio LLC

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
#include <gui/saveshader.hh>
#include <gui/consolelog.hh>
#include <database/locale.hh>
#include <database/shaderdata.hh>
#include <imgui.h>
#include <iostream>

namespace gui {
  void SaveShader::Save(sqlitepp::Instance &db) {
    db::ShaderData saveShader = {};
    saveShader.name = m_Info->shaderName;
    saveShader.description = "";
    saveShader.vertexShader.filename = m_Info->shaderName + ".vert";
    saveShader.vertexShader.type = db::ShaderCodeType::GlslVertex;
    saveShader.vertexShader.code = m_Info->VERT.code;
    saveShader.fragmentShader.filename = m_Info->shaderName + ".frag";
    saveShader.fragmentShader.type = db::ShaderCodeType::GlslFragment;
    saveShader.fragmentShader.code = m_Info->FRAG.code;
    saveShader.luaCode.filename = m_Info->shaderName + ".lua";
    saveShader.luaCode.type = db::ShaderCodeType::Lua;
    saveShader.luaCode.code = m_Info->LUA.code;

    db::SaveNewShader(db, saveShader);

    LogEntry entry;
    entry.text = saveShader.name;
    entry.type = LogType::Normal;
    m_Info->cmd->Add(entry);
  }
} // namespace gui