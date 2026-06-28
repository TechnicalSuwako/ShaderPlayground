/*
# 076 License

Copyright (c) 076 Studio LLC

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
#include <gui/about.hh>
#include <imgui.h>
#include <database/locale.hh>
#include <gui/guiengine.hh>

namespace gui {
  void About::Draw(Info &info) {
    if (!info.isAbout) return;

    ImGuiWindowFlags flags = 0;
    ImGui::SetNextWindowSize({400, 300}, ImGuiCond_Appearing);
    ImGui::Begin(info.i18n->GetWord("helpabout").c_str(), &info.isAbout, flags);
    if (GuiEngine::ShouldCloseWindowOnEscape()) info.isAbout = false;
    string name = "Shader Playground " + info.version;
    string copyright;
    string developer;
    if (info.i18n->GetCurrentLanguage().code == "ja_JP") {
      copyright = "© 2026 - ０７６スタジオ合同会社";
      developer = "開発者：テクニカル諏訪子";
    } else {
      copyright = "© 2026 - 076 Studio LLC";
      developer = "Developer: TechnicalSuwako";
    }

    ImGui::SetWindowFontScale(1.8f);
    ImGui::Text(name.c_str());
    ImGui::SetWindowFontScale(1.2f);
    ImGui::Separator();
    ImGui::Text(copyright.c_str());
    ImGui::Text(developer.c_str());

    f32 btnW = 80.f;
    f32 btnH = 0.f;

    ImVec2 winSize = ImGui::GetWindowSize();
    ImVec2 cursorPos = ImGui::GetCursorPos();

    ImGui::SetCursorPos({
      (winSize.x - btnW) * .5f,
      winSize.y - 40.f
    });

    if (ImGui::Button("OK", { btnW, btnH })) info.isAbout = false;
    ImGui::End();
  }
} // namespace gui