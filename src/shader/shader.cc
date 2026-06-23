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
#include <shader/shader.hh>
#include <stdexcept>

Shader::Shader(const string &src, GLenum type) {
  id = glCreateShader(type);
  const char *s = src.c_str();
  glShaderSource(id, 1, &s, nullptr);
  glCompileShader(id);

  // シェーダーのコンパイルエラーをチェック
  GLint success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(id, 512, NULL, infoLog);
    throw std::runtime_error("シェーダーのコンパイルに失敗しました:\n" + string(infoLog));
  }
}

Shader::~Shader() {
  glDeleteShader(id);
}

Program::Program(Shader vertexShader, Shader fragmentShader) {
  id = glCreateProgram();
  glAttachShader(id, vertexShader.GetShader());
  glAttachShader(id, fragmentShader.GetShader());
  glLinkProgram(id);

  // プログラムのリンクエラーをチェック
  GLint success;
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(id, 512, NULL, infoLog);
    throw std::runtime_error("プログラムのリンクに失敗しました:\n" + string(infoLog));
  }
}

Program::~Program() {
  glDeleteProgram(id);
}

void Program::Use() {
  glUseProgram(id);
}

void Program::ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
  glClearColor(red, green, blue, alpha);
}

void Program::Clear(GLbitfield mask) {
  glClear(GL_COLOR_BUFFER_BIT);
}

GLint Program::GetUniformLocation(string var) {
  return glGetUniformLocation(id, var.c_str());
}

string GetDefaultVertexShader() {
  return R"(
#version 460 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aUv;
out vec2 vUv;

void main() {
  vUv = aUv;
  gl_Position = vec4(aPosition, 0.0, 1.0);
}
)";
}

string GetDefaultFragmentShader() {
  return R"(
#version 460 core

in vec2 vUv;
out vec4 out_color;

uniform vec3 uColorA;
uniform vec3 uColorB;
uniform vec3 uColorC;

void main() {
  vec3 temp = mix(uColorA, uColorB, vUv.x);
  vec3 blendedColor = mix(temp, uColorC, vUv.y * 0.95);
  out_color = vec4(blendedColor, 1.0);
}
)";
}

string GetDefaultLuaCode() {
  return R"(
local mesh = {
  vertices = {
    -1, -1,  0, 0,
     1, -1,  1, 0,
     1,  1,  1, 1,
    -1,  1,  0, 1
  },
  indices = {
    0, 1, 2,
    2, 3, 0
  },

  attributes = {
    { location = 0, size = 2, stride = 16, offset = 0 }, -- position
    { location = 1, size = 2, stride = 16, offset = 0 }  -- uv
  }
}

graphics.set_mesh(mesh)
)";
}
