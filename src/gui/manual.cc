#include <gui/manual.hh>
#include <imgui.h>

namespace gui {

  void Manual::Draw(GlfwInfo &glfw) {
    if (!glfw.isManual) return;

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowSize({ 600, 700 }, ImGuiCond_Appearing);
    ImGui::Begin("マニュアル", &glfw.isManual, flags);

    if (ImGui::BeginTabBar("ManualTabs")) {
      if (ImGui::BeginTabItem("概要")) {
        m_CurPage = "index";
        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem("シューだー")) {
        m_CurPage = "shader";
        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem("Luaエンジン")) {
        m_CurPage = "lua";
        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem("ホットキー")) {
        m_CurPage = "hotkeys";
        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
    }

    ImGui::Separator();

    ImGui::BeginChild("ManualContent", ImVec2(0, 0), 0, ImGuiWindowFlags_HorizontalScrollbar);

    if (m_CurPage == "index") {
      ImGui::TextWrapped("Shader Playgroundへようこそ！");
      ImGui::Dummy(ImVec2(0, 10));
      renderMarkdown(R"(# 基本操作

- **コンパイル**: F5 又は「ビルド→コンパイル」
- **保存**: Ctrl+s 又は「ファイル→保存」
- 右側のパネルでシェーダーコードを編集出来ます
)");
    } else if (m_CurPage == "shader") {
      ImGui::TextWrapped("Shader Playgroundへようこそ！");
      ImGui::Dummy(ImVec2(0, 10));
      renderMarkdown(R"(# 基本操作

- **コンパイル**: F5 又は「ビルド→コンパイル」
- **保存**: Ctrl+s 又は「ファイル→保存」
- 右側のパネルでシェーダーコードを編集出来ます
)");
    } else if (m_CurPage == "lua") {
      ImGui::TextWrapped("Lua");
      ImGui::Dummy(ImVec2(0, 10));
      renderMarkdown(R"(# Luaエンジン

- **コンパイル**: F5 又は「ビルド→コンパイル」
- **保存**: Ctrl+s 又は「ファイル→保存」
- 右側のパネルでシェーダーコードを編集出来ます
)");
    } else if (m_CurPage == "hotkeys") {
      ImGui::TextWrapped("Shader Playgroundへようこそ！");
      ImGui::Dummy(ImVec2(0, 10));
      renderMarkdown(R"(# 基本操作

- **コンパイル**: F5 又は「ビルド→コンパイル」
- **保存**: Ctrl+s 又は「ファイル→保存」
- 右側のパネルでシェーダーコードを編集出来ます
)");
    }

    ImGui::EndChild();
    ImGui::End();
  }

  /////////

  void Manual::renderMarkdown(cstr text) {
    ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
    ImGui::TextUnformatted(text);
    ImGui::PopTextWrapPos();
  }
} // namespace gui