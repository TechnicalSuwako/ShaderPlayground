#pragma once

#include <util/types.hh>
#include <util/structs.hh>

namespace gui {
  class Manual {
    public:
      void Draw(GlfwInfo &glfw);

    private:
      void renderMarkdown(cstr text);
      string m_CurPage = "index";
  }; // class Manual
} // namespace gui