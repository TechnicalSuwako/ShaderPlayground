#include <gui/consolelog.hh>
#include <imgui.h>

namespace gui {
  void ConsoleLog::Add(LogEntry entry) {
    m_Lines.push_back(entry);
  }

  void ConsoleLog::Draw(cstr title) {
    ImGui::Begin(title);
    if (ImGui::Button("消す")) Clear();
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