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
#include <gui/titlebar.hh>
#include <gui/about.hh>

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <ctime>

namespace gui {
  void showTitleBar(GlfwInfo &glfw) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_MenuBar |
      ImGuiWindowFlags_NoDocking;

    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    window_flags |= ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("MainDockSpace", nullptr, window_flags);

    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockspace_id);

    static bool firstLayout = true;
    if (firstLayout) {
      firstLayout = false;

      ImGui::DockBuilderRemoveNode(dockspace_id);
      ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);

      const ImGuiViewport *viewport = ImGui::GetMainViewport();
      ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

      ImGuiID dock_main = dockspace_id;
      ImGuiID dock_right = 0;
      ImGuiID dock_bottom = 0;

      dock_bottom = ImGui::DockBuilderSplitNode(
        dock_main,
        ImGuiDir_Down,
        0.25f,
        nullptr,
        &dock_main
      );

      dock_right = ImGui::DockBuilderSplitNode(
        dock_main,
        ImGuiDir_Right,
        0.35f,
        nullptr,
        &dock_main
      );

      ImGui::DockBuilderDockWindow("View", dock_main);

      ImGui::DockBuilderDockWindow("VertexEditor", dock_right);
      ImGui::DockBuilderDockWindow("FragmentEditor", dock_right);
      ImGui::DockBuilderDockWindow("LuaEditor", dock_right);

      ImGui::DockBuilderDockWindow("Console", dock_bottom);

      ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGui::End();

    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("ファイル")) {
        if (ImGui::MenuItem("新しいファイル", "CTRL+N", false, false)) {}
        if (ImGui::MenuItem("開く", "Ctrl+O", false, false)) {}
        //if (ImGui::BeginMenu("最近使った物")) {
        //  ImGui::MenuItem("fish_hat.c");
        //  ImGui::MenuItem("fish_hat.inl");
        //  ImGui::MenuItem("fish_hat.h");
        //  if (ImGui::BeginMenu("もっと見る...")) {
        //    ImGui::MenuItem("Hello");
        //    ImGui::MenuItem("Sailor");
        //    ImGui::EndMenu();
        //  }
        //  ImGui::EndMenu();
        //}
        if (ImGui::MenuItem("保存", "Ctrl+S")) {
          glfw.save();
        }

        ImGui::Separator();
        if (ImGui::MenuItem("終了", "CTRL+Q")) {
          glfw.isRunning = false;
        }
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("編集")) {
        if (ImGui::MenuItem("元に戻す", "Ctrl+Z", false, false)) {}
        if (ImGui::MenuItem("やり直し", "Ctrl+Y", false, false)) {}
        ImGui::Separator();
        if (ImGui::MenuItem("切り取り", "Ctrl+X", false, false)) {}
        if (ImGui::MenuItem("コピー", "Ctrl+C", false, false)) {}
        if (ImGui::MenuItem("貼り付け", "Ctrl+V", false, false)) {}
        if (ImGui::MenuItem("行を複製", "Ctrl+D", false, false)) {}
        ImGui::Separator();
        if (ImGui::MenuItem("設定", "Ctrl+,", false, false)) {
          glfw.isSettings = true;
        }
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("ビルド")) {
        if (ImGui::MenuItem("コンパイル", "F5")) {
          glfw.compile();
        }
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("ヘルプ")) {
        if (ImGui::MenuItem("マニュアル", "F1")) {
          glfw.isManual = true;
        }
        if (ImGui::MenuItem("Shader Playgroundについて", "CTRL+H")) {
          glfw.isAbout = true;
        }
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("言語/Language")) {
        if (ImGui::MenuItem("日本語")) {}
        if (ImGui::MenuItem("English")) {}
        ImGui::EndMenu();
      }

      ImGui::SameLine();

      f32 rightW = ImGui::GetContentRegionAvail().x;
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + rightW - 400.f);
      ImGui::Text("現在フレームレート： %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

      ImGui::SameLine();
      char timeBuf[64];
      time_t now = time(nullptr);
      tm *local = localtime(&now);
      strftime(timeBuf, sizeof(timeBuf), "%Y年%m月日%d日 %H:%M:%S", local);
      ImGui::Text(" | %s", timeBuf);

      ImGui::EndMainMenuBar();
    }
  }
} // namespace gui