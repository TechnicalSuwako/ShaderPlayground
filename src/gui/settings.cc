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
#include <gui/settings.hh>
#include <gui/consolelog.hh>
#include <util/structs.hh>
#include <database/locale.hh>
#include <imgui.h>

namespace gui {
  void Settings::Draw(GlfwInfo &glfw) {
    if (!glfw.isSettings) return;

    ImGuiWindowFlags flags = 0;
    ImGui::SetNextWindowSize({ 400, 300 }, ImGuiCond_Appearing);
    ImGui::Begin(glfw.i18n->GetWord("settings").c_str(), &glfw.isSettings, flags);

    ImGui::Text("言語 / Language");
    ImGui::SameLine();
    cstr items[] = { "日本語 (ja_JP)", "English (en_US)" };
    const i32 itemCount = IM_COUNTOF(items);
    i32 curId = glfw.i18n->GetCurrentLanguage().id;
    static i32 sel = (curId >= 1) ? curId - 1 : 0;

    sel = (curId >= 1 && curId <= itemCount) ? curId - 1 : 0;

    const char *preview = items[sel];
    if (ImGui::BeginCombo("##Language", preview)) {
      for (int n = 0; n < itemCount; ++n) {
        const bool isSel = (sel == n);
        if (ImGui::Selectable(items[n], isSel)) {
          sel = n;
          glfw.i18n->SetLanguage(n + 1);
          gui::LogEntry entry;
          entry.type = gui::LogType::Info;
          entry.text = glfw.i18n->GetWord("consoleloginfolangchangeok");
          glfw.cmd->Add(entry);
        }

        if (isSel) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }

    f32 btnW = 80.f;
    f32 btnH = 0.f;

    ImVec2 winSize = ImGui::GetWindowSize();
    ImVec2 cursorPos = ImGui::GetCursorPos();

    ImGui::SetCursorPos({
      (winSize.x - btnW) * .5f,
      winSize.y - 40.f
                        });

    if (ImGui::Button("OK", { btnW, btnH })) glfw.isSettings = false;
    ImGui::End();
  }
} // namespace gui