#include <gui/guiengine.hh>
#include <util/glfwpp.hh>

namespace gui {
  GuiEngine::GuiEngine(glfwpp::Window *window) {
    m_Window = window;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    m_CjkFont = io.Fonts->AddFontFromFileTTF("NotoSansCJKjp-Regular.otf", 16.f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    m_MonoFont = io.Fonts->AddFontFromFileTTF("NotoMono-Regular.ttf", 16.f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window->Get(), true);
    ImGui_ImplOpenGL3_Init("#version 460");
  }

  GuiEngine::~GuiEngine() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }
} // namespace gui