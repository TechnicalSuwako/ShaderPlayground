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
#include <database/locale.hh>

namespace gui {
  void Manual::Draw(Info &info) {
    if (!info.isManual) return;

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowSize({ 600, 700 }, ImGuiCond_Appearing);
    ImGui::Begin(info.i18n->GetWord("helpmanual").c_str(), &info.isManual, flags);

    if (ImGui::BeginTabBar("ManualTabs")) {
      if (ImGui::BeginTabItem(info.i18n->GetWord("helpmanualoverview").c_str())) {
        m_CurPage = "index";
        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem(info.i18n->GetWord("helpmanualshader").c_str())) {
        m_CurPage = "shader";
        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem(info.i18n->GetWord("helpmanuallua").c_str())) {
        m_CurPage = "lua";
        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem(info.i18n->GetWord("helpmanualhotkey").c_str())) {
        m_CurPage = "hotkeys";
        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
    }

    ImGui::Separator();

    ImGui::BeginChild("ManualContent", ImVec2(0, 0), 0, ImGuiWindowFlags_HorizontalScrollbar);

    if (m_CurPage == "index") {
      if (info.i18n->GetCurrentLanguage().code == "ja_JP") {
        ImGui::TextWrapped("Shader Playgroundへようこそ！");
        ImGui::Dummy(ImVec2(0, 10));
        renderMarkdown(R"(# 基本操作

**Note**: For English, go to 編集 → 設定 (or press CTRL + ,), and set "言語/Language" to "English".

- **コンパイル**: F5 又は「ビルド→コンパイル」
- **保存**: Ctrl+S 又は「ファイル→保存」
- 右側のパネルでシェーダーコードを編集出来ます
)");
      } else {
        ImGui::TextWrapped("Welcome to Shader Playground!");
        ImGui::Dummy(ImVec2(0, 10));
        renderMarkdown(R"(# Basic operation

**注意**: 日本語で読むには、「Edit → Settings」をクリックし（若しくはCTRL + ,を押し）、「言語/Language」から「日本語」を御選択下さい。".

- **Compile**: F5 or "Build → Compile"
- **Save**: Ctrl+S or "File → Save"
- You can edit the shader code in the right side panel
)");
      }
    } else if (m_CurPage == "shader") {
      ImGui::TextWrapped("Shader Playgroundへようこそ！");
      ImGui::Dummy(ImVec2(0, 10));
      renderMarkdown(R"(# シェーダー
)");
    } else if (m_CurPage == "lua") {
      ImGui::TextWrapped("Lua");
      ImGui::Dummy(ImVec2(0, 10));
      renderMarkdown(R"(# Luaエンジン
)");
    } else if (m_CurPage == "hotkeys") {
      ImGui::TextWrapped("Shader Playgroundへようこそ！");
      ImGui::Dummy(ImVec2(0, 10));
      renderMarkdown(R"(# ホットキー
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