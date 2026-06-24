#include <gui/editor.hh>

namespace gui {
  Editor::Editor(string title, string renderName, string code, Languages lang, ImFont *cjkFont, ImFont *monoFont) {
    m_CjkFont = cjkFont;
    m_MonoFont = monoFont;
    m_Title = title;
    m_RenderName = renderName;
    m_Code = code;
    m_Editor.SetText(code);
    m_Lang = lang;

    if (lang == Glsl) {
      m_Editor.SetLanguage(TextEditor::Language::Glsl());
    } else if (lang == Hlsl) {
      // 現在未対応
      m_Editor.SetLanguage(TextEditor::Language::Hlsl());
    } else if (lang == Lua) {
      m_Editor.SetLanguage(TextEditor::Language::Lua());
    }
  }

  Editor::~Editor() {}

  void Editor::Render() {
    ImGui::PushFont(m_CjkFont);
    ImGui::Begin(m_Title.c_str());
    ImGui::PopFont();
    ImGui::PushFont(m_MonoFont);
    m_Editor.Render(m_RenderName.c_str());
    ImGui::PopFont();
    RenderStatusBar(m_Editor);
    ImGui::End();
  }

  void Editor::RenderStatusBar(TextEditor editor) {
    static const char *languages[] = { "Lua", "GLSL", "HLSL" };

    static const TextEditor::Language *definitions[] = {
      TextEditor::Language::Lua(),
      TextEditor::Language::Glsl(),
      TextEditor::Language::Hlsl(),
    };

    std::string language = editor.GetLanguageName();

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
    ImGui::BeginChild("StatusBar", ImVec2(0.0f, 0.0f), ImGuiChildFlags_Borders);
    ImGui::SetNextItemWidth(120.0f);

    if (ImGui::BeginCombo("##LanguageSelector", language.c_str())) {
      for (int n = 0; n < IM_ARRAYSIZE(languages); n++) {
        bool selected = (language == languages[n]);

        if (ImGui::Selectable(languages[n], selected)) {
          editor.SetLanguage(definitions[n]);
          //buildAutocompleteTrie();
        }

        if (selected) {
          ImGui::SetItemDefaultFocus();
        }
      }

      ImGui::EndCombo();
    }

    ImGui::SameLine(0.0f, 0.0f);
    ImGui::AlignTextToFramePadding();

    int line;
    int column;
    int tabSize = editor.GetTabSize();
    float glyphWidth = ImGui::CalcTextSize("#").x;
    editor.GetCurrentCursor(line, column);

    // determine status message
    char status[256];

    auto statusSize = std::snprintf(
      status,
      sizeof(status),
      "Ln %d, Col %d  Tab Size: %d  File: %s",
      line + 1,
      column + 1,
      tabSize,
      "default" // ファイル名
    );

    auto size = glyphWidth * (statusSize + 3);
    auto width = ImGui::GetContentRegionAvail().x;

    ImGui::SameLine(0.0f, width - size);
    ImGui::TextUnformatted(status);

    ImGui::SameLine(0.0f, glyphWidth * 1.0f);
    auto drawlist = ImGui::GetWindowDrawList();
    auto pos = ImGui::GetCursorScreenPos();
    auto offset = ImGui::GetFrameHeight() * 0.5f;
    auto radius = offset * 0.6f;
    //auto color = isDirty() ? IM_COL32(164, 0, 0, 255) : IM_COL32(164, 164, 164, 255);
    auto color = IM_COL32(164, 0, 0, 255);
    drawlist->AddCircleFilled(ImVec2(pos.x + offset, pos.y + offset), radius, color);

    ImGui::EndChild();
    ImGui::PopStyleColor();
  }
} // namespace gui