#pragma once

#include <util/types.hh>

#include <imgui.h>
#include <TextEditor.h>

namespace gui {
  enum Languages {
    Glsl,
    Hlsl,
    Lua,
  };

  class Editor {
    public:
      Editor(string title, string renderName, string code, Languages lang, ImFont *cjkFont, ImFont *monoFont);
      ~Editor();

      TextEditor Get() const { return m_Editor; }
      void Render();
      void SetCode(string code);
      void RenderStatusBar(TextEditor editor);

    private:
      TextEditor m_Editor;
      ImFont *m_CjkFont;
      ImFont *m_MonoFont;
      string m_Title;
      string m_RenderName;
      string m_Code;
      Languages m_Lang;
  }; // class Editor
} // namespace gui