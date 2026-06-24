#include <gui/titlebar.hh>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace gui {
  void showTitleBar() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (ImGui::BeginMainMenuBar()) {
      if (ImGui::BeginMenu("ファイル")) {
        if (ImGui::MenuItem("新しいファイル")) {}
        if (ImGui::MenuItem("開く", "Ctrl+O")) {}
        if (ImGui::BeginMenu("最近使った物")) {
          ImGui::MenuItem("fish_hat.c");
          ImGui::MenuItem("fish_hat.inl");
          ImGui::MenuItem("fish_hat.h");
          if (ImGui::BeginMenu("もっと見る...")) {
            ImGui::MenuItem("Hello");
            ImGui::MenuItem("Sailor");
            ImGui::EndMenu();
          }
          ImGui::EndMenu();
        }
        if (ImGui::MenuItem("保存", "Ctrl+S")) {}

        ImGui::Separator();
        if (ImGui::MenuItem("終了", "CTRL+Q")) {}
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("編集")) {
        if (ImGui::MenuItem("元に戻す", "Ctrl+Z")) {}
        if (ImGui::MenuItem("やり直し", "Ctrl+Y", false, false)) {}
        ImGui::Separator();
        if (ImGui::MenuItem("切り取り", "Ctrl+X")) {}
        if (ImGui::MenuItem("コピー", "Ctrl+C")) {}
        if (ImGui::MenuItem("貼り付け", "Ctrl+V")) {}
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
  }
} // namespace gui