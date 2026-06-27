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
        Make(gui::ShaderType::Simple2D);
        m_Info->create();
      }
    }
  }

  db::ShaderData NewShader::Make(ShaderType type) {
    db::ShaderData newShader = {};
    auto curLang = m_Info->i18n->GetCurrentLanguage().code;
    newShader.id = 0;
    if (curLang == "ja_JP") newShader.name = "(名無し)";
    else newShader.name = "(Unnamed)";
    newShader.description = "";

    newShader.vertexShader.id = 0;
    newShader.vertexShader.type = db::ShaderCodeType::GlslVertex;
    newShader.vertexShader.filename = "";

    newShader.fragmentShader.id = 0;
    newShader.fragmentShader.type = db::ShaderCodeType::GlslFragment;
    newShader.fragmentShader.filename = "";

    newShader.luaCode.id = 0;
    newShader.luaCode.type = db::ShaderCodeType::Lua;
    newShader.luaCode.filename = "";

    if (type == ShaderType::Scene) {
      newShader.vertexShader.code = getDefaultVertScene();
      newShader.fragmentShader.code = getDefaultFragScene();
      newShader.luaCode.code = getDefaultLuaScene();
    } else if (type == ShaderType::Simple3D) {
      newShader.vertexShader.code = getDefaultVert3D();
      newShader.fragmentShader.code = getDefaultFrag3D();
      newShader.luaCode.code = getDefaultLua3D();
    } else {
      newShader.vertexShader.code = getDefaultVert2D();
      newShader.fragmentShader.code = getDefaultFrag2D();
      newShader.luaCode.code = getDefaultLua2D();
    }

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

le.gfx.add_mesh(mesh)
le.gfx.set_position(0, 0, 0, 0)

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

le.gfx.add_mesh(mesh)
le.gfx.set_position(0, 0, 0, 1)
le.gfx.add_mesh(mesh)
le.gfx.set_position(1, -8, -2, -11)
le.gfx.add_mesh(mesh)
le.gfx.set_position(2, 8, -2, -11)

function init()
end

function update()
  local t = le.sys.get_time()

  le.gfx.set_rotation(0, t*0.7, t*1.0, t*0.4)
  le.gfx.set_rotation(1, t*0.2, t*0.8, t*1.4)
  le.gfx.set_rotation(2, t*0.2, t*0.8, t*1.4)

  local reso = le.sys.get_resolution()
  local aspect = reso.x / reso.y

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

  le.gfx.set_uniform_mat4("uView", view)
  le.gfx.set_uniform_mat4("uProjection", projection)
end)";
  }

  string NewShader::getDefaultVertScene() {
    return R"(#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;

out vec3 vFragPos;
out vec3 vNormal;
out vec3 vColor;
out vec4 vFragPosLightSpace;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uLightSpace;

void main() {
  vec4 worldPos = uModel * vec4(aPosition, 1.0);

  vFragPos = worldPos.xyz;
  vNormal = mat3(transpose(inverse(uModel))) * aNormal;
  vColor = aColor;
  vFragPosLightSpace = uLightSpace * worldPos;

  gl_Position = uProjection * uView * worldPos;
})";
  }

  string NewShader::getDefaultFragScene() {
    return R"(#version 460 core

in vec3 vFragPos;
in vec3 vNormal;
in vec3 vColor;
in vec4 vFragPosLightSpace;

out vec4 out_color;

uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform sampler2D uShadowMap;

void main() {
  vec3 norm = normalize(vNormal);
  vec3 lightDir = normalize(uLightPos - vFragPos);

  float diff = max(dot(norm, lightDir), 0.0);

  vec3 projCoords = vFragPosLightSpace.xyz / vFragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;

  float closestDepth = texture(uShadowMap, projCoords.xy).r;
  float currentDepth = projCoords.z;

  float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);

  float shadow =
    (projCoords.z > 1.0)
    ? 0.0
    : (currentDepth - bias > closestDepth ? 1.0 : 0.0);

  vec3 ambient = 0.2 * vColor;
  vec3 diffuse = (1.0 - shadow) * diff * uLightColor * vColor;

  vec3 viewDir = normalize(-vFragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
  vec3 specular = (1.0 - shadow) * 0.3 * spec * uLightColor;

  vec3 result = ambient + diffuse + specular;

  out_color = vec4(result, 1.0);
})";
  }

  string NewShader::getDefaultLuaScene() {
    return R"(local posSize = le.sizeof("vec3")
local normalSize = le.sizeof("vec3")
local colorSize = le.sizeof("vec3")

local stride = posSize + normalSize + colorSize

local sphere = {
  vertices = {},

  indices = {},

  attributes = {
    { location = 0, size = 3, stride = stride, offset = 0 },
    { location = 1, size = 3, stride = stride, offset = posSize },
    { location = 2, size = 3, stride = stride, offset = posSize + normalSize }
  }
}

local plane = {
  vertices = {
    -20, -2, -20,   0,1,0,   0.4, 0.4, 0.4,
     20, -2, -20,   0,1,0,   0.4, 0.4, 0.4,
     20, -2,  20,   0,1,0,   0.4, 0.4, 0.4,
    -20, -2,  20,   0,1,0,   0.4, 0.4, 0.4,
  },
  indices = {
    0, 1, 2,
    0, 2, 3,
  },
  attributes = {
    { location = 0, size = 3, stride = stride, offset = 0 },
    { location = 1, size = 3, stride = stride, offset = posSize },
    { location = 2, size = 3, stride = stride, offset = posSize + normalSize }
  }
}

local pyramid = {
  vertices = {
     0.0,  0.8,  0.0,    0.0,  0.447,  0.894,   1,1,1,
    -0.5, -0.2,  0.5,    0.0,  0.447,  0.894,   1,0,0,
     0.5, -0.2,  0.5,    0.0,  0.447,  0.894,   0,1,0,

     0.0,  0.8,  0.0,    0.894,  0.447,  0.0,   1,1,1,
     0.5, -0.2,  0.5,    0.894,  0.447,  0.0,   0,1,0,
     0.5, -0.2, -0.5,    0.894,  0.447,  0.0,   0,0,1,

     0.0,  0.8,  0.0,    0.0,  0.447, -0.894,   1,1,1,
     0.5, -0.2, -0.5,    0.0,  0.447, -0.894,   0,0,1,
    -0.5, -0.2, -0.5,    0.0,  0.447, -0.894,   1,1,0,

     0.0,  0.8,  0.0,   -0.894,  0.447,  0.0,   1,1,1,
    -0.5, -0.2, -0.5,   -0.894,  0.447,  0.0,   1,1,0,
    -0.5, -0.2,  0.5,   -0.894,  0.447,  0.0,   1,0,0,

    -0.5, -0.2,  0.5,    0,-1,0,   1,0,0,
     0.5, -0.2,  0.5,    0,-1,0,   0,1,0,
     0.5, -0.2, -0.5,    0,-1,0,   0,0,1,
    -0.5, -0.2, -0.5,    0,-1,0,   1,1,0,
  },

  indices = {
    0,1,2,
    3,4,5,
    6,7,8,
    9,10,11,

    12,13,14,
    12,14,15
  },

  attributes = {
    { location = 0, size = 3, stride = stride, offset = 0 },
    { location = 1, size = 3, stride = stride, offset = posSize },
    { location = 2, size = 3, stride = stride, offset = posSize + normalSize }
  }
}

local cube = {
  vertices = {
    -- position        --normal      -- uv
    -- mae
    -0.5, -0.5, -0.5,   0,  0, -1,    1,0,0,
     0.5, -0.5, -0.5,   0,  0, -1,    0,1,0,
     0.5,  0.5, -0.5,   0,  0, -1,    0,0,1,
    -0.5,  0.5, -0.5,   0,  0, -1,    1,1,0,
    -- usiro
    -0.5, -0.5,  0.5,   0,  0,  1,    1,0,1,
     0.5, -0.5,  0.5,   0,  0,  1,    0,1,1,
     0.5,  0.5,  0.5,   0,  0,  1,    1,1,1,
    -0.5,  0.5,  0.5,   0,  0,  1,    1,0,1,
    -- (migi)
     0.5, -0.5, -0.5,   1,  0,  0,    0,1,0,
     0.5, -0.5,  0.5,   1,  0,  0,    0,1,1,
     0.5,  0.5,  0.5,   1,  0,  0,    1,1,1,
     0.5,  0.5, -0.5,   1,  0,  0,    0,0,1,
    -- hidari
    -0.5, -0.5,  0.5,  -1,  0,  0,    1,0,1,
    -0.5, -0.5, -0.5,  -1,  0,  0,    1,0,0,
    -0.5,  0.5, -0.5,  -1,  0,  0,    1,1,0,
    -0.5,  0.5,  0.5,  -1,  0,  0,    1,0,1,
    -- ue
    -0.5,  0.5, -0.5,   0,  1,  0,    1,1,0,
     0.5,  0.5, -0.5,   0,  1,  0,    0,0,1,
     0.5,  0.5,  0.5,   0,  1,  0,    1,1,1,
    -0.5,  0.5,  0.5,   0,  1,  0,    1,0,1,
    -- sita
    -0.5, -0.5, -0.5,   0, -1,  0,    1,0,0,
     0.5, -0.5, -0.5,   0, -1,  0,    0,1,0,
     0.5, -0.5,  0.5,   0, -1,  0,    0,1,1,
    -0.5, -0.5,  0.5,   0, -1,  0,    1,0,1,
  },

  indices = {
    0,1,2, 2,3,0,
    4,5,6, 6,7,4,
    8,9,10, 10,11,8,
    12,13,14, 14,15,12,
    16,17,18, 18,19,16,
    20,21,22, 22,23,20
  },

  attributes = {
    { location = 0, size = 3, stride = stride, offset = 0 },
    { location = 1, size = 3, stride = stride, offset = posSize },
    { location = 2, size = 3, stride = stride, offset = posSize + normalSize }
  }
}

local radius = .5
local stacks = 16
local slices = 24

for i = 0, stacks do
  local phi = math.pi * i / stacks
  local y = radius * math.cos(phi)
  local ringRadius = radius * math.sin(phi)

  for j = 0, slices -1 do
    local theta = 2 * math.pi * j / slices

    local x = ringRadius * math.cos(theta)
    local z = ringRadius * math.sin(theta)

    local len = math.sqrt(x*x + y*y + z*z)
    local nx = x / len
    local ny = y / len
    local nz = z / len

    local r = (x + radius) / (2 * radius)
    local g = (y + radius) / (2 * radius)
    local b = (z + radius) / (2 * radius)

    table.insert(sphere.vertices, x)
    table.insert(sphere.vertices, y)
    table.insert(sphere.vertices, z)

    table.insert(sphere.vertices, nx)
    table.insert(sphere.vertices, ny)
    table.insert(sphere.vertices, nz)

    table.insert(sphere.vertices, r)
    table.insert(sphere.vertices, g)
    table.insert(sphere.vertices, b)
  end
end
  
for i = 0, stacks - 1 do
  for j = 0, slices - 1 do
    local nextJ = (j + 1) % slices

    local a = i * slices + j
    local b = (i + 1) * slices + j
    local c = i * slices + nextJ
    local d = (i + 1) * slices + nextJ

    table.insert(sphere.indices, a)
    table.insert(sphere.indices, b)
    table.insert(sphere.indices, c)

    table.insert(sphere.indices, c)
    table.insert(sphere.indices, b)
    table.insert(sphere.indices, d)
  end
end

le.gfx.add_mesh(cube)
le.gfx.set_position(0, 0, 0, -1)
le.gfx.add_mesh(cube)
le.gfx.set_position(1, -8, -2, -11)
le.gfx.add_mesh(cube)
le.gfx.set_position(2, 8, -2, -11)

le.gfx.add_mesh(pyramid)
le.gfx.set_position(3, 8, 2, -11)
le.gfx.set_scale(3, 2, 2, 2)

le.gfx.add_mesh(sphere)
le.gfx.set_position(4, -2, 1, -3)
  
le.gfx.add_mesh(plane)
le.gfx.set_position(5, 0, -2, 0)

function init()
end

function update()
  local t = le.sys.get_time()
  local zs = -3 + math.sin(t * 1.5) * 6
  le.gfx.set_position(4, -2.3, -1, zs)

  local size = 8
  local speed = 0.8
  local phase = (t * speed) % (4 * math.pi)
  local cubeX, cubeY

  if phase < math.pi then
    cubeX = -size/2 + (phase / math.pi) * size
    cubeY = size/2
  elseif phase < 2 * math.pi then
    cubeX = size/2
    cubeY = size/2 - ((phase - math.pi) / math.pi) * size
  elseif phase < 3 * math.pi then
    cubeX = size/2 - ((phase - 2*math.pi) / math.pi) * size
    cubeY = -size/2
  else
    cubeX = -size/2
    cubeY = -size/2 + ((phase - 3*math.pi) / math.pi) * size
  end

  le.gfx.set_position(2, cubeX, cubeY, -11)
  
  le.gfx.set_rotation(0, t*0.7, t*1.0, t*0.4)
  le.gfx.set_rotation(1, t*0.2, t*0.8, t*1.4)
  le.gfx.set_rotation(2, t*0.4, t*0.5, t*0.8)
  le.gfx.set_rotation(3, 0, t*0.8, -.5)
  
  local reso = le.sys.get_resolution()
  local aspect = reso.x / reso.y

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

  le.gfx.set_uniform_mat4("uView", view)
  le.gfx.set_uniform_mat4("uProjection", projection)
  le.gfx.set_uniform_vec3("uLightPos", 3, 5, 2)
  le.gfx.set_uniform_vec3("uLightColor", 1, 1, 1)
end)";
  }
} // namespace gui