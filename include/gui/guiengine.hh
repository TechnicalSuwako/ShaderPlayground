#pragma once

#include <util/types.hh>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace glfwpp {
  class Window;
} // namespace glfwpp

namespace gui {
  class GuiEngine {
    public:
      GuiEngine(glfwpp::Window *window);
      ~GuiEngine();

      ImGuiIO &GetIO() const { return ImGui::GetIO(); }
      ImFont *GetCjkFont() const { return m_CjkFont; }
      ImFont *GetMonoFont() const { return m_MonoFont; }

    private:
      glfwpp::Window *m_Window;
      ImFont *m_CjkFont;
      ImFont *m_MonoFont;
  }; // class GuiEngine
} // namespace gui