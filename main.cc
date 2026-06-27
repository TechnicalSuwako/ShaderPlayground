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

  // シャドーマッピング
#pragma region shadowmap
  constexpr u32 SHADOW_WIDTH = 1024;
  constexpr u32 SHADOW_HEIGHT = 1024;

  GLuint shadowFBO;
  glGenFramebuffers(1, &shadowFBO);

  GLuint shadowMap;
  glGenTextures(1, &shadowMap);
  glBindTexture(GL_TEXTURE_2D, shadowMap);

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_DEPTH_COMPONENT,
    SHADOW_WIDTH,
    SHADOW_HEIGHT,
    0,
    GL_DEPTH_COMPONENT,
    GL_FLOAT,
    nullptr
  );

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  float borderColor[] = { 1.f, 1.f, 1.f, 1.f };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
  glFramebufferTexture2D(
    GL_FRAMEBUFFER,
    GL_DEPTH_ATTACHMENT,
    GL_TEXTURE_2D,
    shadowMap,
    0
  );

  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  string depthVertSource = R"(#version 460 core
layout(location = 0) in vec3 aPosition;

uniform mat4 uLightSpace;
uniform mat4 uModel;

void main() {
  gl_Position = uLightSpace * uModel * vec4(aPosition, 1.0);
})";

  string depthFragSource = R"(#version 460 core
void main() { })";
#pragma endregion

  // 3Dを有効
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  //glEnable(GL_CULL_FACE);

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
  info.pendingCompile = true;
  auto &objects = luaEngine.GetObjects();

  VertexArrays VAO(1);
  Buffers VBO(Buffer, 1);
  Buffers EBO(ArrayBuffer, 1);

  for (auto &obj : objects) {
    VAO.BindVertexArray();

    VBO.BindBuffer();
    VBO.BufferData(obj.mesh.vertices.data(), obj.mesh.vertices.size() * sizeof(f32));

    EBO.BindBuffer();
    EBO.BufferData(obj.mesh.indices.data(), obj.mesh.indices.size() * sizeof(GLuint));

    // 頂点属性
    for (const auto &a : obj.mesh.attr) {
      glVertexAttribPointer(a.location, a.size, GL_FLOAT, GL_FALSE, a.stride, (void *)(uintptr_t)a.offset);
      glEnableVertexAttribArray(a.location);
    }
  }

  bool isHold = false;
  gui::About about;
  gui::Settings settings;

#pragma region shadowmap
  Shader depthVertex(depthVertSource, GL_VERTEX_SHADER);
  Shader depthFragment(depthFragSource, GL_FRAGMENT_SHADER);
  Program depthProgram(depthVertex, depthFragment);
#pragma endregion

  // メインレンダリングループ
  while (!window.ShouldClose() && info.isRunning) {
#pragma region Lambda
    info.compile = [&]() {
      if (!info.pendingCompile) return;
      info.pendingCompile = false;
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
        shaderProgram.Use();
        luaEngine.SetProgram(&shaderProgram);
        luaEngine.Reload(info.LUA.code);
        {
          auto &newObj = luaEngine.GetObjects();
          for (auto &obj : newObj) {
            if (obj.mesh.vertices.empty() || obj.mesh.indices.empty()) {
              string msg;
              if (info.i18n->GetCurrentLanguage().code == "ja_JP") msg = "メッシュデータが空です。";
              else msg = "Mesh data is empty.";
              throw std::runtime_error(msg);
            }

            VAO.BindVertexArray();

            VBO.BindBuffer();
            VBO.BufferData(
              obj.mesh.vertices.data(),
              obj.mesh.vertices.size() * sizeof(f32)
            );

            EBO.BindBuffer();
            EBO.BufferData(
              obj.mesh.indices.data(),
              obj.mesh.indices.size() * sizeof(u32)
            );

            for (const auto &a : obj.mesh.attr) {
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
      if (ImGui::BeginPopupModal(info.i18n->GetWord("filenewcreateshader").c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (ImGui::Button(info.i18n->GetWord("filenew2dshader").c_str(), ImVec2(200, 0))) {
          gui::NewShader newShader(&info);
          auto shdr = newShader.Make(gui::ShaderType::Simple2D);
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

          info.pendingCompile = true;

          gui::LogEntry entry;
          entry.type = gui::LogType::Info;
          entry.text = info.i18n->GetWord("consoleloginfocreateok");
          info.cmd->Add(entry);
          std::cout << entry.text << std::endl;

          ImGui::CloseCurrentPopup();
        }

        if (ImGui::Button(info.i18n->GetWord("filenew3dshader").c_str(), ImVec2(200, 0))) {
          gui::NewShader newShader(&info);
          auto shdr = newShader.Make(gui::ShaderType::Simple3D);
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

          info.pendingCompile = true;

          gui::LogEntry entry;
          entry.type = gui::LogType::Info;
          entry.text = info.i18n->GetWord("consoleloginfocreateok");
          info.cmd->Add(entry);
          std::cout << entry.text << std::endl;

          ImGui::CloseCurrentPopup();
        }

        if (ImGui::Button(info.i18n->GetWord("filenewsceneshader").c_str(), ImVec2(200, 0))) {
          gui::NewShader newShader(&info);
          auto shdr = newShader.Make(gui::ShaderType::Scene);
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

          info.pendingCompile = true;

          gui::LogEntry entry;
          entry.type = gui::LogType::Info;
          entry.text = info.i18n->GetWord("consoleloginfocreateok");
          info.cmd->Add(entry);
          std::cout << entry.text << std::endl;

          ImGui::CloseCurrentPopup();
        }

        ImGui::Separator();

        if (ImGui::Button(info.i18n->GetWord("cancel").c_str())) {
          ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
      }
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
        info.pendingCompile = true;
      }

      if (isNewKey) {
        info.showNewShaderPopup = true;
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

    if (info.showNewShaderPopup) {
      ImGui::OpenPopup(info.i18n->GetWord("filenewcreateshader").c_str());
      info.showNewShaderPopup = false;
    }

    info.create();
    info.compile();

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

#pragma region shadowmap
    Matrix4 lightProjection = mat4Ortho(
      -20.f, 20.f,
      -20.f, 20.f,
      0.1f, 100.f
    );

    Matrix4 lightView = mat4LookAt(
      { 10.f, 10.f, 10.f },   // 光の位置
      { 0.f, 0.f, 0.f },      // ターゲット
      { 0.f, 1.f, 0.f }       // 上
    );

    Matrix4 lightSpace = mat4Mul(lightProjection, lightView);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    depthProgram.Use();
    depthProgram.SetUniformMat4("uLightSpace", lightSpace);

    for (auto &obj : objects) {
      if (obj.mesh.vertices.empty()) continue;
      if (obj.mesh.indices.empty()) continue;

      Matrix4 T = mat4Translate(
        obj.position.pos.x,
        obj.position.pos.y,
        obj.position.pos.z
      );

      Matrix4 R = mat4Rotate(
        obj.rotation.pos.x,
        obj.rotation.pos.y,
        obj.rotation.pos.z
      );

      Matrix4 S = mat4Scale(
        obj.scale.pos.x,
        obj.scale.pos.y,
        obj.scale.pos.z
      );

      Matrix4 model = mat4Mul(T, mat4Mul(R, S));

      depthProgram.SetUniformMat4("uModel", model);

      VAO.BindVertexArray();
      VBO.BindBuffer();
      VBO.BufferData(obj.mesh.vertices.data(),
                     obj.mesh.vertices.size() * sizeof(f32));

      EBO.BindBuffer();
      EBO.BufferData(obj.mesh.indices.data(),
                     obj.mesh.indices.size() * sizeof(GLuint));

      glDrawElements(
        GL_TRIANGLES,
        obj.mesh.GetIndexCount(),
        GL_UNSIGNED_INT,
        nullptr
      );
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

    // 画面をクリア
    glBindFramebuffer(GL_FRAMEBUFFER, viewport.GetFBO());
    glViewport(0, 0, viewport.GetWidth(), viewport.GetHeight());
    shaderProgram.ClearColor(0.f, 0.f, 0.f, 1.f);
    shaderProgram.Clear();

    // シェーダープログラムを使用
    shaderProgram.Use();

#pragma region shadowmap
    shaderProgram.SetUniformMat4("uLightSpace", lightSpace);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    shaderProgram.SetUniformInt("uShadowMap", 0);
#pragma endregion

    luaEngine.Init();
    luaEngine.Update();

    auto &objects = luaEngine.GetObjects();
    for (auto &obj : objects) {
      if (obj.mesh.vertices.empty()) continue;
      if (obj.mesh.indices.empty()) continue;

      // モデルマトリックスの作成
      Matrix4 T = mat4Translate(
        obj.position.pos.x,
        obj.position.pos.y,
        obj.position.pos.z
      );

      Matrix4 R = mat4Rotate(
        obj.rotation.pos.x,
        obj.rotation.pos.y,
        obj.rotation.pos.z
      );

      Matrix4 S = mat4Scale(
        obj.scale.pos.x,
        obj.scale.pos.y,
        obj.scale.pos.z
      );

      Matrix4 model = mat4Mul(T, mat4Mul(R, S));
      glUniformMatrix4fv(shaderProgram.GetUniformLocation("uModel"), 1, GL_FALSE, model.m);

      // 描画
      VAO.BindVertexArray();
      VBO.BindBuffer();
      VBO.BufferData(obj.mesh.vertices.data(), obj.mesh.vertices.size() * sizeof(f32));
      EBO.BindBuffer();
      EBO.BufferData(obj.mesh.indices.data(), obj.mesh.indices.size() * sizeof(GLuint));

      glDrawElements(GL_TRIANGLES, obj.mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
    }
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