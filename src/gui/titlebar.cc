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
#include <gui/titlebar.hh>

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <database/locale.hh>
#include <ctime>

namespace gui {
  void showTitleBar(Info &info) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoCollapse |
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

    ImGui::GetIO().ConfigDockingWithShift = true;
    ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_None;

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
      if (ImGui::BeginMenu(info.i18n->GetWord("file").c_str())) {
        if (ImGui::MenuItem(info.i18n->GetWord("filenew").c_str(), "CTRL+N")) {
          info.create();
        }
        if (ImGui::MenuItem(info.i18n->GetWord("fileopen").c_str(), "Ctrl+O", false, false)) {}
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
        if (ImGui::MenuItem(info.i18n->GetWord("filesave").c_str(), "Ctrl+S")) {
          info.save();
        }

        ImGui::Separator();
        if (ImGui::MenuItem(info.i18n->GetWord("quit").c_str(), "CTRL+Q")) {
          info.isRunning = false;
        }
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu(info.i18n->GetWord("edit").c_str())) {
        if (ImGui::MenuItem(info.i18n->GetWord("editundo").c_str(), "Ctrl+Z", false, false)) {}
        if (ImGui::MenuItem(info.i18n->GetWord("editredo").c_str(), "Ctrl+Y", false, false)) {}
        ImGui::Separator();
        if (ImGui::MenuItem(info.i18n->GetWord("editcut").c_str(), "Ctrl+X", false, false)) {}
        if (ImGui::MenuItem(info.i18n->GetWord("editcopy").c_str(), "Ctrl+C", false, false)) {}
        if (ImGui::MenuItem(info.i18n->GetWord("editpaste").c_str(), "Ctrl+V", false, false)) {}
        if (ImGui::MenuItem(info.i18n->GetWord("editduplicate").c_str(), "Ctrl+D", false, false)) {}
        ImGui::Separator();
        if (ImGui::MenuItem(info.i18n->GetWord("settings").c_str(), "Ctrl+,")) {
          info.isSettings = true;
        }
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu(info.i18n->GetWord("build").c_str())) {
        if (ImGui::MenuItem(info.i18n->GetWord("buildcompile").c_str(), "F5")) {
          info.compile();
        }
        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu(info.i18n->GetWord("help").c_str())) {
        if (ImGui::MenuItem(info.i18n->GetWord("helpmanual").c_str(), "F1")) {
          info.isManual = true;
        }
        if (ImGui::MenuItem(info.i18n->GetWord("helpabout").c_str(), "CTRL+H")) {
          info.isAbout = true;
        }
        ImGui::EndMenu();
      }

      ImGui::SameLine();

      f32 side;
      if (info.i18n->GetCurrentLanguage().code == "ja_JP") side = 379.f;
      else side = 380.f;

      ImGui::SetCursorPosX(ImGui::GetWindowWidth() - side);

      string fmt = info.i18n->GetWord("currentfps") + "： %.3f ms/frame (%.1f FPS)";
      ImGui::Text(fmt.c_str(), 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

      ImGui::SameLine();
      ImGui::Text(" | ");
      ImGui::SameLine();

      char timeBuf[64];
      time_t now = time(nullptr);
      tm *local = localtime(&now);

      strftime(timeBuf, sizeof(timeBuf), "%Y/%m/%d %H:%M:%S", local);
      ImGui::Text("%s", timeBuf);

      ImGui::EndMainMenuBar();
    }
  }
} // namespace gui