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
#include <util/glfwpp.hh>
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
#include <gui/guiengine.hh>
#include <gui/titlebar.hh>
#include <gui/editor.hh>
#include <gui/consolelog.hh>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <sol.hpp>

constexpr int windowWidth = 1280;
constexpr int windowHeight = 720;
string sofname = "Shader Playground";

int main(void) {
  // GLFWを初期化
  glfwpp::Instance glfw;
  glfw.SetErrorCallback();
  glfw.SetContextMajorVersion(4);
  glfw.SetContextMinorVersion(6);
  glfw.SetOpenGLProfile(glfwpp::Attributes::OpenGLCore);
  glfw.SetForwardCompatibility(true);
  glfw.CreateWindowHints();

  // ウィンドウを作成
  string winName = sofname + " " + version::full;
  f32 mainScale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
  glfwpp::Window window((windowWidth * mainScale), (windowHeight * mainScale), winName);
  window.MakeContextCurrent();
  glfw.SwapInterval(1);

#ifndef PRODUCTION_BUILD
  bool showDemo = true;
#endif

  // GLADを初期化
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "GLADの初期化に失敗しました" << std::endl;
    return -1;
  }

  // ImGui設定
  gui::GuiEngine ge(&window);

  // フレームバッファーサイズ
  window.SetFramebufferSizeCallback();

  // データベースが未だ存在しなければ、初期設定
  sqlite::Instance db;
  db::Initialize(db);

  // コンソール
  gui::ConsoleLog cmd;

  // データベースからシェーダーとLuaコードの受け取り
  vector<db::ShaderData> codeMap = db::GetAllShaders(db);
  u32 shaderId;
  string shaderName;
  string VERT = "";
  string FRAG = "";
  string LUA = "";

  for (const auto &c : codeMap) {
    shaderId = c.id;
    shaderName = c.name;
    VERT = c.vertexShader.code;
    FRAG = c.fragmentShader.code;
    LUA = c.luaCode.code;
  }

  string title = shaderName + "（頂点シェーダー）";
  gui::Editor vertEditor(title, "VertexEditor", VERT, gui::Glsl, ge.GetCjkFont(), ge.GetMonoFont());

  title = shaderName + "（フラグメントシェーダー）";
  gui::Editor fragEditor(title, "FragmentEditor", FRAG, gui::Glsl, ge.GetCjkFont(), ge.GetMonoFont());

  title = shaderName + "（Luaエディター）";
  gui::Editor luaEditor(title, "LuaEditor", LUA, gui::Lua, ge.GetCjkFont(), ge.GetMonoFont());

  // シェーダーをコンパイル
  Shader vertexShader(VERT, GL_VERTEX_SHADER);
  Shader fragmentShader(FRAG, GL_FRAGMENT_SHADER);

  // シェーダープログラムをリンク
  Program shaderProgram(vertexShader, fragmentShader);

  // 頂点データはLuaから受け取る
  lua::LuaEngine luaEngine(LUA, &shaderProgram, &cmd);
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

  bool isHold = false;

  // メインレンダリングループ
  while (!window.ShouldClose()) {
    auto compileCommand = [&]() {
      VERT = vertEditor.Get().GetText();
      FRAG = fragEditor.Get().GetText();
      LUA = luaEditor.Get().GetText();

      try {
        shaderProgram.Reload(VERT, FRAG);
        luaEngine.SetProgram(&shaderProgram);
        luaEngine.Reload(LUA);
        {
          lua::LuaMesh newMesh = luaEngine.GetMesh();

          VAO.BindVertexArray();

          VBO.BindBuffer();
          VBO.BufferData(
            newMesh.vertices.data(),
            newMesh.vertices.size() * sizeof(f32)
          );

          EBO.BindBuffer();
          EBO.BufferData(
            newMesh.indices.data(),
            newMesh.indices.size() * sizeof(f32)
          );

          for (const auto &a : newMesh.attr) {
            glVertexAttribPointer(
              a.location,
              a.size,
              GL_FLOAT,
              GL_FALSE,
              a.stride,
              (any)(uintptr_t)a.offset
            );
            glEnableVertexAttribArray(a.location);
          }
        }
      } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
      }

      gui::LogEntry entry;
      entry.type = gui::LogType::Info;
      entry.text = "シェーダーをコンパイルしました。";
      cmd.Add(entry);
      std::cout << entry.text << std::endl;
    };

    auto saveCommand = [&]() {
      VERT = vertEditor.Get().GetText();
      FRAG = fragEditor.Get().GetText();
      LUA = luaEditor.Get().GetText();

      db::SaveCode(db, shaderId, VERT, "vertex_shader");
      db::SaveCode(db, shaderId, FRAG, "fragment_shader");
      db::SaveCode(db, shaderId, LUA, "lua_code");

      gui::LogEntry entry;
      entry.type = gui::LogType::Info;
      entry.text = "シェーダー及びLuaファイルを保存しました。";
      cmd.Add(entry);
      std::cout << entry.text << std::endl;
    };

    glfw.PollEvents();

    bool isCompileKey = window.GetKey(GLFW_KEY_F5);
    bool ctrlMod = (window.GetKey(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || window.GetKey(GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS);
    bool isSaveKey = (window.GetKey(GLFW_KEY_S) == GLFW_PRESS && ctrlMod);

    if (!isHold) {
      if (isCompileKey) {
        //isHold = true;
        compileCommand();
      }

      if (isSaveKey) {
        //isHold = true;
        saveCommand();
      }
    }

    isHold = isSaveKey || isCompileKey;

    if (window.GetAttrib(glfwpp::Attributes::Iconified) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    gui::showTitleBar();

#ifndef PRODUCTION_BUILD
    if (showDemo) ImGui::ShowDemoWindow(&showDemo);
#endif

    vertEditor.Render();
    fragEditor.Render();
    luaEditor.Render();
    cmd.Draw();

    {
      static float f = 0.0f;
      static int counter = 0;

      ImGui::Begin("コントロール");

      if (ImGui::Button("コンパイル（F5）")) {
        compileCommand();
      }

      ImGui::SameLine();

      if (ImGui::Button("保存（CTRL + S）")) {
        saveCommand();
      }

      ImGui::Text("現在フレームレート： %.3f ms/frame (%.1f FPS)", 1000.0f / ge.GetIO().Framerate, ge.GetIO().Framerate);
      ImGui::End();
    }

    ImGui::Render();

    // 画面をクリア
    shaderProgram.ClearColor(0.f, 0.f, 0.f, 1.f);
    shaderProgram.Clear();

    // シェーダープログラムを使用
    shaderProgram.Use();
    luaEngine.Update();

    // クアッドを描画
    VAO.BindVertexArray();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // バッファをスワップし、イベントを処理
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    window.SwapBuffers();
  }

  return 0;
}
