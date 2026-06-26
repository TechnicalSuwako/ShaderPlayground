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
#include <util.hh>
#include <shader.hh>
#include <database.hh>
#include <lua.hh>
#include <gui.hh>
#include <version.hh>
#include <iostream>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <sol.hpp>
#include <chrono>

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
  glfw.SetMaximized(true);
  glfw.CreateWindowHints();

  u32 seed = static_cast<u32>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  util::rand::Seed(seed);

  // ウィンドウを作成
  string winName = sofname + " " + version::full;
  f32 mainScale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
  glfwpp::Window window((windowWidth * mainScale), (windowHeight * mainScale), winName);
  window.MakeContextCurrent();
  glfw.SwapInterval(1);

  Info info = { &glfw, &window, true, false, false, false };

#ifndef PRODUCTION_BUILD
  bool showDemo = true;
#endif

  // GLADを初期化
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "GLADの初期化に失敗しました" << std::endl;
    return -1;
  }

  // ImGui設定
  gui::GuiEngine ge(&window);

  // フレームバッファーサイズ
  window.SetFramebufferSizeCallback();

  // データベースが未だ存在しなければ、初期設定
  sqlitepp::Instance db;
  try {
    db::Initialize(db);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  // 言語
  db::Locale i18n(&db);
  info.i18n = &i18n;

  // コンソール
  gui::ConsoleLog cmd;
  info.cmd = &cmd;

  // ビューポート
  gui::ViewPort viewport;
  viewport.Init();

  // マニュアル
  gui::Manual manual;

  // データベースからシェーダーとLuaコードの受け取り
  vector<db::ShaderData> codeMap = db::GetAllShaders(db);
  info.shaderId;
  info.shaderName;
  info.VERT = {};
  info.FRAG = {};
  info.LUA = {};

  for (const auto &c : codeMap) {
    info.shaderId = c.id;
    info.shaderName = c.name;
    info.VERT = c.vertexShader;
    info.FRAG = c.fragmentShader;
    info.LUA = c.luaCode;
  }

  string title = info.shaderName + "（" + info.i18n->GetWord("editorvertshader") + "）";
  gui::Editor vertEditor((title + "###VertexEditor").c_str(), "VertexEditor", info.VERT.code, gui::Glsl, ge.GetCjkFont(), ge.GetMonoFont());

  title = info.shaderName + "（" + info.i18n->GetWord("editorfragshader") + "）";
  gui::Editor fragEditor((title + "###FragmentEditor").c_str(), "FragmentEditor", info.FRAG.code, gui::Glsl, ge.GetCjkFont(), ge.GetMonoFont());

  title = info.shaderName + "（" + info.i18n->GetWord("editorlua") + "）";
  gui::Editor luaEditor((title + "###LuaEditor").c_str(), "LuaEditor", info.LUA.code, gui::Lua, ge.GetCjkFont(), ge.GetMonoFont());

  // シェーダーをコンパイル
  Shader vertexShader(info.VERT.code, GL_VERTEX_SHADER);
  Shader fragmentShader(info.FRAG.code, GL_FRAGMENT_SHADER);

  // シェーダープログラムをリンク
  Program shaderProgram(vertexShader, fragmentShader);

  // 頂点データはLuaから受け取る
  lua::LuaEngine luaEngine(info.LUA.code, &shaderProgram, &info);
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
  gui::About about;
  gui::Settings settings;

  // メインレンダリングループ
  while (!window.ShouldClose() && info.isRunning) {
#pragma region Lambda
    info.compile = [&]() {
      if (luaEngine.GetInitializationState()) luaEngine.SetInitializationState(false);
      info.VERT.code = vertEditor.Get().GetText();
      info.FRAG.code = fragEditor.Get().GetText();
      info.LUA.code = luaEditor.Get().GetText();

      try {
        try {
          if (!luaEngine.Validate(info.LUA.code)) {
            gui::LogEntry entry;
            entry.type = gui::LogType::Error;
            entry.text = info.i18n->GetWord("consolelogerrorluainvalidcomp");
            info.cmd->Add(entry);
            std::cerr << entry.text << std::endl;
            return;
          }
        } catch (const std::exception &e) {
          gui::LogEntry entry;
          entry.type = gui::LogType::Error;
          entry.text = e.what();
          info.cmd->Add(entry);
          std::cerr << entry.text << std::endl;
          entry.text = info.i18n->GetWord("consolelogerrorluainvalidcomp");
          info.cmd->Add(entry);
          std::cerr << entry.text << std::endl;
          return;
        }

        shaderProgram.Reload(info.VERT.code, info.FRAG.code);
        luaEngine.SetProgram(&shaderProgram);
        luaEngine.Reload(info.LUA.code);
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
            newMesh.indices.size() * sizeof(u32)
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

        gui::LogEntry entry;
        entry.type = gui::LogType::Info;
        entry.text = info.i18n->GetWord("consoleloginfocompileok");
        info.cmd->Add(entry);
        std::cout << entry.text << std::endl;
      } catch (const std::exception &e) {
        gui::LogEntry entry;
        entry.type = gui::LogType::Error;
        entry.text = e.what();
        info.cmd->Add(entry);
        std::cerr << entry.text << std::endl;
        entry.text = info.i18n->GetWord("consolelogerrorluainvalidcomp");
        info.cmd->Add(entry);
        std::cerr << entry.text << std::endl;
      }
    };

    info.save = [&]() {
      if (info.shaderId == 0) {
        gui::LogEntry entry;
        entry.type = gui::LogType::Warning;
        std::cout << entry.text << std::endl;
        entry.text = "TODO: 保存ダイアログボックス関数は開発中・・・";
        info.cmd->Add(entry);
        std::cout << entry.text << std::endl;
        return;
      }

      info.VERT.code = vertEditor.Get().GetText();
      info.FRAG.code = fragEditor.Get().GetText();
      info.LUA.code = luaEditor.Get().GetText();

      try {
        try {
          if (!luaEngine.Validate(info.LUA.code)) {
            gui::LogEntry entry;
            entry.type = gui::LogType::Error;
            entry.text = info.i18n->GetWord("consolelogerrorluainvalidsave");
            info.cmd->Add(entry);
            std::cout << entry.text << std::endl;
            return;
          }
        } catch (const std::exception &e) {
          gui::LogEntry entry;
          entry.type = gui::LogType::Error;
          entry.text = e.what();
          info.cmd->Add(entry);
          std::cout << entry.text << std::endl;
          entry.text = info.i18n->GetWord("consolelogerrorluainvalidsave");
          info.cmd->Add(entry);
          std::cout << entry.text << std::endl;
          return;
        }

        db::SaveCode(db, info.VERT);
        db::SaveCode(db, info.FRAG);
        db::SaveCode(db, info.LUA);

        gui::LogEntry entry;
        entry.type = gui::LogType::Info;
        entry.text = info.i18n->GetWord("consoleloginfosaveok");
        info.cmd->Add(entry);
        std::cout << entry.text << std::endl;
      } catch (const std::exception &e) {
        gui::LogEntry entry;
        entry.type = gui::LogType::Error;
        entry.text = e.what();
        info.cmd->Add(entry);
        std::cout << entry.text << std::endl;
        entry.text = info.i18n->GetWord("consolelogerrorluainvalidsave");
        info.cmd->Add(entry);
        std::cout << entry.text << std::endl;
      }
    };

    info.create = [&]() {
      gui::NewShader newShader(&info);
      auto shdr = newShader.Make();

      info.shaderId = shdr.id;
      info.shaderName = shdr.name;
      info.VERT = shdr.vertexShader;
      info.FRAG = shdr.fragmentShader;
      info.LUA = shdr.luaCode;

      vertEditor.SetCode(info.VERT.code);
      string nTit = info.shaderName + "（" + info.i18n->GetWord("editorvertshader") + "）";
      vertEditor.SetTitle(nTit + "###VertexEditor");

      fragEditor.SetCode(info.FRAG.code);
      nTit = info.shaderName + "（" + info.i18n->GetWord("editorfragshader") + "）";
      fragEditor.SetTitle(nTit + "###FragmentEditor");

      luaEditor.SetCode(info.LUA.code);
      nTit = info.shaderName + "（" + info.i18n->GetWord("editorlua") + "）";
      luaEditor.SetTitle(nTit + "###LuaEditor");

      info.compile();

      gui::LogEntry entry;
      entry.type = gui::LogType::Info;
      entry.text = info.i18n->GetWord("consoleloginfocreateok");
      info.cmd->Add(entry);
      std::cout << entry.text << std::endl;
    };
#pragma endregion

    glfw.PollEvents();

    bool isManualKey = window.GetKey(GLFW_KEY_F1);
    bool isCompileKey = window.GetKey(GLFW_KEY_F5);

    bool ctrlMod = (window.GetKey(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || window.GetKey(GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS);
    bool shiftMod = (window.GetKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || window.GetKey(GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
    //bool altMod = (window.GetKey(GLFW_KEY_LEFT_ALT) == GLFW_PRESS || window.GetKey(GLFW_KEY_RIGHT_ALT) == GLFW_PRESS);

    bool isNewKey = (window.GetKey(GLFW_KEY_N) == GLFW_PRESS && ctrlMod);
    bool isOpenKey = (window.GetKey(GLFW_KEY_O) == GLFW_PRESS && ctrlMod);
    bool isSaveKey = (window.GetKey(GLFW_KEY_S) == GLFW_PRESS && ctrlMod);
    bool isSaveAsKey = (window.GetKey(GLFW_KEY_S) == GLFW_PRESS && ctrlMod && shiftMod);
    bool isQuitKey = (window.GetKey(GLFW_KEY_Q) == GLFW_PRESS && ctrlMod);
    bool isAboutKey = (window.GetKey(GLFW_KEY_H) == GLFW_PRESS && ctrlMod);
    bool isSettingsKey = (window.GetKey(GLFW_KEY_COMMA) == GLFW_PRESS && ctrlMod);

    if (!isHold) {
      if (isCompileKey) {
        info.compile();
      }

      if (isNewKey) {
        info.create();
      }

      if (isOpenKey) {}

      if (isManualKey) {
        info.isManual = true;
      }

      if (isSettingsKey) {
        info.isSettings = true;
      }

      if (isAboutKey) {
        info.isAbout = true;
      }

      if (isSaveKey) {
        info.save();
      }

      if (isQuitKey) {
        info.isRunning = false;
      }
    }

    isHold = isSaveKey ||
             isCompileKey ||
             isAboutKey ||
             isQuitKey ||
             isManualKey ||
             isSettingsKey ||
             isNewKey ||
             isOpenKey;

    if (info.isLangChange) {
      string nTit = info.shaderName + "（" + info.i18n->GetWord("editorvertshader") + "）";
      vertEditor.SetTitle(nTit + "###VertexEditor");
      nTit = info.shaderName + "（" + info.i18n->GetWord("editorfragshader") + "）";
      fragEditor.SetTitle(nTit + "###FragmentEditor");
      nTit = info.shaderName + "（" + info.i18n->GetWord("editorlua") + "）";
      luaEditor.SetTitle(nTit + "###LuaEditor");
      info.isLangChange = false;
    }

    if (window.GetAttrib(glfwpp::Attributes::Iconified) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    gui::showTitleBar(info);

#ifndef PRODUCTION_BUILD
    if (showDemo) ImGui::ShowDemoWindow(&showDemo);
#endif

    vertEditor.Render();
    fragEditor.Render();
    luaEditor.Render();
    info.cmd->Draw(info, info.i18n->GetWord("editorconsole").c_str());
    about.Draw(info);
    settings.Draw(info);
    manual.Draw(info);

    viewport.Draw(info);

    ImGui::Render();

    // 画面をクリア
    glBindFramebuffer(GL_FRAMEBUFFER, viewport.GetFBO());
    glViewport(0, 0, viewport.GetWidth(), viewport.GetHeight());
    shaderProgram.ClearColor(0.f, 0.f, 0.f, 1.f);
    shaderProgram.Clear();

    // シェーダープログラムを使用
    shaderProgram.Use();
    luaEngine.Init();
    luaEngine.Update();

    // クアッドを描画
    VAO.BindVertexArray();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // バッファをスワップし、イベントを処理
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow *backup_current_context = glfwGetCurrentContext();

      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();

      glfwMakeContextCurrent(backup_current_context);
    }

    window.SwapBuffers();
  }

  return 0;
}