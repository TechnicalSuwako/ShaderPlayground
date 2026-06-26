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
#include <gui/newshader.hh>
#include <database/locale.hh>

namespace gui {
  NewShader::NewShader(Info *info) : m_Info(info) {}
  NewShader::~NewShader() {}

  db::ShaderData NewShader::Make() {
    db::ShaderData newShader = {};
    auto curLang = m_Info->i18n->GetCurrentLanguage().code;
    newShader.id = 0;
    if (curLang == "ja_JP") newShader.name = "(名無し)";
    else newShader.name = "(Unnamed)";
    newShader.description = "";

    newShader.vertexShader.id = 0;
    newShader.vertexShader.type = db::ShaderCodeType::GlslVertex;
    newShader.vertexShader.code = getDefaultVert();
    newShader.vertexShader.filename = "";

    newShader.fragmentShader.id = 0;
    newShader.fragmentShader.type = db::ShaderCodeType::GlslFragment;
    newShader.fragmentShader.code = getDefaultFrag();
    newShader.fragmentShader.filename = "";

    newShader.luaCode.id = 0;
    newShader.luaCode.type = db::ShaderCodeType::Lua;
    newShader.luaCode.code = getDefaultLua();
    newShader.luaCode.filename = "";

    return newShader;
  }
  
  ///////////////////////////

  string NewShader::getDefaultVert() {
    return R"(#version 460 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;
out vec3 vColor;

void main() {
  vColor = aColor;
  gl_Position = vec4(aPosition, 0.0, 1.0);
})";
  }
  
  string NewShader::getDefaultFrag() {
    return R"(#version 460 core

in vec3 vColor;
out vec4 out_color;

void main() {
  out_color = vec4(vColor, 1.0);
})";
  }

  string NewShader::getDefaultLua() {
    return R"(local mesh = {
  vertices = {
     0,  .5,   1, 0, 0,
   -.5, -.5,   0, 1, 0,
    .5, -.5,   0, 0, 1
  },
  indices = { 0, 1, 2 },
  
  attributes = {
    { location = 0, size = 2, stride = 20, offset = 0 }, -- position
    { location = 1, size = 3, stride = 20, offset = 8 } -- uv
  }
}
  
le.gfx.set_mesh(mesh)
  
function update()
end)";
  }
} // namespace gui