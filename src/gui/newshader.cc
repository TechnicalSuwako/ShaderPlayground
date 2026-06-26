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
#include <imgui.h>

namespace gui {
  NewShader::NewShader(Info *info) : m_Info(info) {}
  NewShader::~NewShader() {}

  void NewShader::DrawPopup() {
    if (ImGui::BeginPopupModal("NewShaderPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
      ImGui::Text(m_Info->i18n->GetWord("filenewcreateshader").c_str());
      ImGui::Separator();

      if (ImGui::Button("2D Shader", ImVec2(200, 0))) {
        Make(false);
        m_Info->create();
      }
    }
  }

  db::ShaderData NewShader::Make(bool is3D) {
    db::ShaderData newShader = {};
    auto curLang = m_Info->i18n->GetCurrentLanguage().code;
    newShader.id = 0;
    if (curLang == "ja_JP") newShader.name = "(名無し)";
    else newShader.name = "(Unnamed)";
    newShader.description = "";

    newShader.vertexShader.id = 0;
    newShader.vertexShader.type = db::ShaderCodeType::GlslVertex;
    newShader.vertexShader.code = is3D ? getDefaultVert3D() : getDefaultVert2D();
    newShader.vertexShader.filename = "";

    newShader.fragmentShader.id = 0;
    newShader.fragmentShader.type = db::ShaderCodeType::GlslFragment;
    newShader.fragmentShader.code = is3D ? getDefaultFrag3D() : getDefaultFrag2D();
    newShader.fragmentShader.filename = "";

    newShader.luaCode.id = 0;
    newShader.luaCode.type = db::ShaderCodeType::Lua;
    newShader.luaCode.code = is3D ? getDefaultLua3D() : getDefaultLua2D();
    newShader.luaCode.filename = "";

    return newShader;
  }
  
  ///////////////////////////

  string NewShader::getDefaultVert2D() {
    return R"(#version 460 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;
out vec3 vColor;

void main() {
  vColor = aColor;
  gl_Position = vec4(aPosition, 0.0, 1.0);
})";
  }
  
  string NewShader::getDefaultFrag2D() {
    return R"(#version 460 core

in vec3 vColor;
out vec4 out_color;

void main() {
  out_color = vec4(vColor, 1.0);
})";
  }

  string NewShader::getDefaultLua2D() {
    return R"(local stride = le.sizeof("vec2") + le.sizeof("vec3")

local mesh = {
  vertices = {
     0,  .5,   1, 0, 0,
   -.5, -.5,   0, 1, 0,
    .5, -.5,   0, 0, 1
  },
  indices = { 0, 1, 2 },
  
  attributes = {
    {
      location = 0,
      size = 2,
      stride = stride,
      offset = 0
    }, -- position
    {
      location = 1,
      size = 3,
      stride = stride,
      offset = le.sizeof("vec2")
    } -- uv
  }
}
  
le.gfx.set_mesh(mesh)

function init()
end
  
function update()
end)";
  }

  string NewShader::getDefaultVert3D() {
    return R"(#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

out vec3 vColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
  vColor = aColor;
  gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
})";
  }

  string NewShader::getDefaultFrag3D() {
    return R"(#version 460 core

in vec3 vColor;
out vec4 out_color;

void main() {
  out_color = vec4(vColor, 1.0);
})";
  }

  string NewShader::getDefaultLua3D() {
    return R"(local offset = le.sizeof("vec3")
local stride = offset * 2

local mesh = {
  vertices = {
    -- position          -- uv
    -- mae
    -0.5, -0.5, -0.5,    1,0,0,
     0.5, -0.5, -0.5,    0,1,0,
     0.5,  0.5, -0.5,    0,0,1,
    -0.5,  0.5, -0.5,    1,1,0,
    -- usiro
    -0.5, -0.5,  0.5,    1,0,1,
     0.5, -0.5,  0.5,    0,1,1,
     0.5,  0.5,  0.5,    1,1,1,
    -0.5,  0.5,  0.5,    1,0,1,
  },

  indices = {
    -- mae
    0,1,2, 2,3,0,
    -- migi
    1,5,6, 6,2,1,
    -- usiro
    5,4,7, 7,6,5,
    -- hidari
    4,0,3, 3,7,4,
    -- ue
    3,2,6, 6,7,3,
    -- sita
    4,5,1, 1,0,4
  },

  attributes = {
    { location = 0, size = 3, stride = stride, offset = 0 },
    { location = 1, size = 3, stride = stride, offset = offset }
  }
}

le.gfx.set_mesh(mesh)

function init()
end

function update()
  local t = le.sys.get_time()
  local reso = le.sys.get_resolution()
  local aspect = reso.x / reso.y

  local model = le.math.rotate(
    t * 0.7,
    t * 1.0,
    t * 0.4
  )

  local view = le.math.look_at(
    0, 0, 4,   -- eye
    0, 0, 0,   -- target
    0, 1, 0    -- up
  )

  local projection = le.math.perspective(
    45.0,
    aspect,
    0.1,
    100.0
  )

  le.gfx.set_uniform_mat4("uModel", model)
  le.gfx.set_uniform_mat4("uView", view)
  le.gfx.set_uniform_mat4("uProjection", projection)
end)";
  }
} // namespace gui