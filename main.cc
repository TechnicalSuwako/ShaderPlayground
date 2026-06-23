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
#include <glad/glad.h>
#include <util/glfw.hh>
#include <iostream>
#include <version.hh>
#include <util/types.hh>
#include <util/vector.hh>
#include <util/sqlite.hh>
#include <shader/shader.hh>
#include <shader/buffers.hh>
#include <database/setup.hh>
#include <database/shaderdata.hh>
#include <lua/luaengine.hh>

#include <sol.hpp>

constexpr int windowWidth = 800;
constexpr int windowHeight = 600;
string sofname = "Shader Playground";

int main(void) {
  // GLFWを初期化
  glfw::Instance glfwInstance;
  glfwInstance.SetContextMajorVersion(4);
  glfwInstance.SetContextMinorVersion(6);
  glfwInstance.SetOpenGLProfile(glfw::OpenGLProfile::Core);
  glfwInstance.SetForwardCompatibility(true);
  glfwInstance.CreateWindowHints();

  // ウィンドウを作成
  string winName = sofname + " " + version::full;
  glfw::Window window(windowWidth, windowHeight, winName);

  // GLADを初期化
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "GLADの初期化に失敗しました" << std::endl;
    return -1;
  }

  window.SetFramebufferSizeCallback();

  // データベースが未だ存在しなければ、初期設定
  sqlite::Instance db;
  db::Initialize(db);

  // データベースからシェーダーとLuaコードの受け取り
  vector<db::ShaderData> codeMap = db::GetAllShaders(db);
  string VERT = "";
  string FRAG = "";
  string LUA = "";

  for (const auto &c : codeMap) {
    VERT = c.vertexShader.code;
    FRAG = c.fragmentShader.code;
    LUA = c.luaCode.code;
  }

  // シェーダーをコンパイル
  Shader vertexShader(VERT, GL_VERTEX_SHADER);
  Shader fragmentShader(FRAG, GL_FRAGMENT_SHADER);

  // シェーダープログラムをリンク
  Program shaderProgram(vertexShader, fragmentShader);

  // 頂点データはLuaから受け取る
  lua::LuaEngine luaEngine(LUA);
  auto &mesh = luaEngine.GetMesh();

  VertexArrays VAO(1);
  Buffers VBO(Buffer, 1);
  Buffers EBO(ArrayBuffer, 1);

  VAO.BindVertexArray();
  VBO.BindBuffer();
  VBO.BufferData(mesh.vertices.data(), mesh.vertices.size() * sizeof(f32));
  EBO.BindBuffer();
  EBO.BufferData(mesh.indices.data(), mesh.indices.size() * sizeof(GLuint));

  // 頂点属性
  for (const auto &a : mesh.attr) {
    glVertexAttribPointer(a.location, a.size, GL_FLOAT, GL_FALSE, a.stride, (void *)(uintptr_t)a.offset);
    glEnableVertexAttribArray(a.location);
  }

  // メインレンダリングループ
  while (!window.ShouldClose()) {
    // 画面をクリア
    shaderProgram.ClearColor(0.f, 0.f, 0.f, 1.f);
    shaderProgram.Clear();

    // シェーダープログラムを使用
    shaderProgram.Use();

    // 時間に基づいて色をアニメーション
    f32 time = (f32)window.GetTime();
    f32 speed = 1.0f; // アニメーションの速度
    f32 phase = 2.0f * 3.14159f / 3.0f; // // 120度の位相差（3色用）

    // uColorA: 赤 -> 緑 -> 青
    Vector3 colorA;
    colorA.r = sin(time * speed + 0.0f) * 0.5f + 0.5f; // 0.0 to 1.0
    colorA.g = sin(time * speed + phase) * 0.5f + 0.5f;
    colorA.b = sin(time * speed + 2.0f * phase) * 0.5f + 0.5f;

    // uColorB: 緑 -> 青 -> 赤
    Vector3 colorB;
    colorB.r = sin(time * speed + 2.0f * phase) * 0.5f + 0.5f;
    colorB.g = sin(time * speed + 0.0f) * 0.5f + 0.5f;
    colorB.b = sin(time * speed + phase) * 0.5f + 0.5f;

    // uColorC: 青 -> 赤 -> 緑
    Vector3 colorC;
    colorC.r = sin(time * speed + phase) * 0.5f + 0.5f;
    colorC.g = sin(time * speed + 2.0f * phase) * 0.5f + 0.5f;
    colorC.b = sin(time * speed + 0.0f) * 0.5f + 0.5f;

    // ユニフォームを設定
    glUniform3f(shaderProgram.GetUniformLocation("uColorA"), colorA.r, colorA.g, colorA.b); // 赤
    glUniform3f(shaderProgram.GetUniformLocation("uColorB"), colorB.r, colorB.g, colorB.b); // 緑
    glUniform3f(shaderProgram.GetUniformLocation("uColorC"), colorC.r, colorC.g, colorC.b); // 青

    // クアッドを描画
    VAO.BindVertexArray();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // バッファをスワップし、イベントを処理
    window.SwapBuffers();
    window.PollEvents();
  }

  return 0;
}