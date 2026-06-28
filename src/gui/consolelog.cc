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
#include <gui/consolelog.hh>
#include <database/locale.hh>
#include <util/structs.hh>
#include <imgui.h>

namespace gui {
  void ConsoleLog::Add(LogEntry entry) {
    m_Lines.push_back(entry);
  }

  void ConsoleLog::Draw(Info &info, cstr title) {
    ImGui::Begin((string(title) + "###Console").c_str());
    if (ImGui::Button(info.i18n->GetWord("editorconsoleclear").c_str())) Clear();
    ImGui::Separator();
    for (const auto &line : m_Lines) {
      if (line.type == LogType::Error)
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", line.text.c_str());
      else if (line.type == LogType::Warning)
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", line.text.c_str());
      else if (line.type == LogType::Info)
        ImGui::TextColored(ImVec4(0, 1, 1, 1), "%s", line.text.c_str());
      else
        ImGui::TextWrapped("%s", line.text.c_str());
    }
    ImGui::End();
  }

  void ConsoleLog::Clear() {
    m_Lines.clear();
  }
} // namespace gui