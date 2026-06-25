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