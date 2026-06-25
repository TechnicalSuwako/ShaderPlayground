/*
# 076 Free License

Copyright (c) テクニカル諏訪子

Permission is hereby granted to any person obtaining a copy of the software
C++ Utils (the "Software") to use, modify, merge, copy, publish, distribute,
sublicense, and/or sell copies of the Software, subject to the following conditions:

    1. **Origin Attribution**:
       - You must not misrepresent the origin of the Software; you must not claim
         you created the original Software.
    2. **Notice Preservation**:
       - This license and the above copyright notice must remain intact in all copies
         of the source code.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <util/glfwpp.hh>
#include <iostream>

namespace glfwpp {
  Instance::Instance() {
    if (!glfwInit()) {
      throw std::runtime_error("GLFWの初期化に失敗しました");
    }
  }

  Instance::~Instance() {
    glfwTerminate();
  }

  static void glfw_error_callback(int error, cstr description) {
    std::cerr << "GLFWエラー " << error << "：\n" << description << std::endl;
  }

  GLFWerrorfun Instance::SetErrorCallback() {
    glfwSetErrorCallback(glfw_error_callback);
    return nullptr;
  }

  void Instance::SetContextMajorVersion(int major) {
    m_Major = major;
  }
  
  void Instance::SetContextMinorVersion(int minor) {
    m_Minor = minor;
  }
  
  void Instance::SetOpenGLProfile(int profile) {
    m_Profile = profile;
  }

  void Instance::SetForwardCompatibility(bool fc) {
    m_ForwardCompat = fc;
  }

  void Instance::SetMaximized(bool fc) {
    m_IsMaximized = fc;
  }

  void Instance::CreateWindowHints() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_Major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_Minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, m_Profile);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, m_ForwardCompat);
    glfwWindowHint(GLFW_MAXIMIZED, m_IsMaximized);
  }

  void Instance::PollEvents() {
    glfwPollEvents();
  }

  void Instance::SwapInterval(int interval) {
    glfwSwapInterval(interval);
  }

  double Instance::GetTime() {
    return glfwGetTime();
  }

  void Instance::SetTime(double time) {
    return glfwSetTime(time);
  }

  u64 Instance::GetTimerValue() {
    return glfwGetTimerValue();
  }

  u64 Instance::GetTimerFrequency() {
    return glfwGetTimerFrequency();
  }

  void Instance::InitHint(int hint, int value) {
    return glfwInitHint(hint, value);
  }

  void Instance::GetVersion(int *major, int *minor, int *rev) {
    glfwGetVersion(major, minor, rev);
  }

  cstr Instance::GetVersionString(void) {
    return glfwGetVersionString();
  }

  int Instance::GetError(cstr *description) {
    return glfwGetError(description);
  }

  GLFWerrorfun Instance::SetErrorCallback(GLFWerrorfun cbfun) {
    return glfwSetErrorCallback(cbfun);
  }

  /////////////

  Window::Window(int width, int height, string title) {
    m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    m_Width = width;
    m_Height = height;
    if (!m_Window) {
      throw std::runtime_error("GLFWウィンドウの作成に失敗しました");
    }

    glfwMakeContextCurrent(m_Window);
  }

  Window::~Window() {
    glfwDestroyWindow(m_Window);
  }

  void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
  }

  GLFWframebuffersizefun Window::SetFramebufferSizeCallback() {
    glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
    return nullptr;
  }

  bool Window::ShouldClose() const {
    return glfwWindowShouldClose(m_Window);
  }

  void Window::SwapBuffers() {
    glfwSwapBuffers(m_Window);
  }

  void Window::GetSize(int *w, int *h) {
    glfwGetWindowSize(m_Window, w, h);
  }

  void Window::Maximize() {
    glfwMaximizeWindow(m_Window);
  }

  void Window::SetClipboardString(string str) {
    glfwSetClipboardString(m_Window, str.c_str());
  }

  string Window::GetClipboardString() {
    return string(glfwGetClipboardString(m_Window));
  }

  void Window::MakeContextCurrent() {
    glfwMakeContextCurrent(m_Window);
  }

  int Window::GetAttrib(int attrib) {
    return glfwGetWindowAttrib(m_Window, attrib);
  }

  int Window::GetInputMode(int mode) {
    return glfwGetInputMode(m_Window, mode);
  }

  void Window::SetInputMode(int mode, int value) {
    glfwSetInputMode(m_Window, mode, value);
  }

  int Window::RawMouseMotionSupported() {
    return glfwRawMouseMotionSupported();
  }

  cstr Window::GetKeyName(int key, int scancode) {
    return glfwGetKeyName(key, scancode);
  }

  int Window::GetKeyScancode(int key) {
    return glfwGetKeyScancode(key);
  }

  int Window::GetKey(int key) {
    return glfwGetKey(m_Window, key);
  }

  int Window::GetMouseButton(int button) {
    return glfwGetMouseButton(m_Window, button);
  }

  void Window::GetCursorPos(double *xpos, double *ypos) {
    return glfwGetCursorPos(m_Window, xpos, ypos);
  }

  void Window::SetCursorPos(double xpos, double ypos) {
    return glfwSetCursorPos(m_Window, xpos, ypos);
  }

  GLFWwindow *Window::GetCurrentContext() {
    return glfwGetCurrentContext();
  }

  bool Window::IsExtensionSupported(cstr ext) {
    return glfwExtensionSupported(ext);
  }

  GLFWglproc Window::GetProcAddress(cstr procname) {
    return glfwGetProcAddress(procname);
  }

  GLFWkeyfun Window::SetKeyCallback(GLFWkeyfun cbfun) {
    return glfwSetKeyCallback(m_Window, cbfun);
  }

  GLFWcharfun Window::SetCharCallback(GLFWcharfun cbfun) {
    return glfwSetCharCallback(m_Window, cbfun);
  }

  GLFWcharmodsfun Window::SetCharModsCallback(GLFWcharmodsfun cbfun) {
    return glfwSetCharModsCallback(m_Window, cbfun);
  }

  GLFWmousebuttonfun Window::SetMouseButtonCallback(GLFWmousebuttonfun cbfun) {
    return glfwSetMouseButtonCallback(m_Window, cbfun);
  }

  GLFWcursorposfun Window::SetCursorPosCallback(GLFWcursorposfun cbfun) {
    return glfwSetCursorPosCallback(m_Window, cbfun);
  }

  GLFWcursorenterfun Window::SetCursorEnterCallback(GLFWcursorenterfun cbfun) {
    return glfwSetCursorEnterCallback(m_Window, cbfun);
  }

  GLFWscrollfun Window::SetScrollCallback(GLFWscrollfun cbfun) {
    return glfwSetScrollCallback(m_Window, cbfun);
  }

  GLFWdropfun Window::SetDropCallback(GLFWdropfun cbfun) {
    return glfwSetDropCallback(m_Window, cbfun);
  }

  int Window::JoystickPresent(int jid) {
    return glfwJoystickPresent(jid);
  }

  const f32 *Window::GetJoystickAxes(int jid, int *count) {
    return glfwGetJoystickAxes(jid, count);
  }

  const unsigned char *Window::GetJoystickButtons(int jid, int *count) {
    return glfwGetJoystickButtons(jid, count);
  }

  const unsigned char *Window::GetJoystickHats(int jid, int *count) {
    return glfwGetJoystickHats(jid, count);
  }

  cstr Window::GetJoystickName(int jid) {
    return glfwGetJoystickName(jid);
  }

  cstr Window::GetJoystickGUID(int jid) {
    return glfwGetJoystickGUID(jid);
  }

  void Window::SetJoystickUserPointer(int jid, any ptr) {
    return glfwSetJoystickUserPointer(jid, ptr);
  }

  any Window::GetJoystickUserPointer(int jid) {
    return glfwGetJoystickUserPointer(jid);
  }

  GLFWjoystickfun Window::SetJoystickCallback(GLFWjoystickfun cbfun) {
    return glfwSetJoystickCallback(cbfun);
  }

  int Window::UpdateGamepadMappings(string str) {
    return glfwUpdateGamepadMappings(str.c_str());
  }

  int Window::JoystickIsGamepad(int jid) {
    return glfwJoystickIsGamepad(jid);
  }

  cstr Window::GetGamepadName(int jid) {
    return glfwGetGamepadName(jid);
  }

  int Window::GetGamepadState(int jid, GLFWgamepadstate *state) {
    return glfwGetGamepadState(jid, state);
  }

  ///////////////////

  std::span<GLFWmonitor *> Monitor::GetMonitors(int *count) {
    if (count) *count = 0;
    GLFWmonitor **mon = glfwGetMonitors(count);
    if (mon == nullptr || count == nullptr || *count <= 0) return {};
    return std::span<GLFWmonitor *>(mon, mon + *count);
  }

  GLFWmonitor *Monitor::GetPrimary() {
    return glfwGetPrimaryMonitor();
  }

  void Monitor::GetPos(int *xpos, int *ypos) {
    glfwGetMonitorPos(m_Monitor, xpos, ypos);
  }

  void Monitor::GetWorkarea(int *xpos, int *ypos, int *width, int *height) {
    glfwGetMonitorWorkarea(m_Monitor, xpos, ypos, width, height);
  }

  void Monitor::GetPhysicalSize(int *width, int *height) {
    glfwGetMonitorPhysicalSize(m_Monitor, width, height);
  }

  void Monitor::GetContentScale(float *xscale, float *yscale) {
    glfwGetMonitorContentScale(m_Monitor, xscale, yscale);
  }

  cstr Monitor::GetName() {
    return glfwGetMonitorName(m_Monitor);
  }

  void Monitor::SetUserPointer(any ptr) {
    glfwSetMonitorUserPointer(m_Monitor, ptr);
  }

  any Monitor::GetUserPointer() {
    return glfwGetMonitorUserPointer(m_Monitor);
  }

  GLFWmonitorfun Monitor::SetCallback(GLFWmonitorfun cbfun) {
    return glfwSetMonitorCallback(cbfun);
  }

  const GLFWvidmode *Monitor::GetVideoModes(int *count) {
    return glfwGetVideoModes(m_Monitor, count);
  }
  
  const GLFWvidmode *Monitor::GetVideoMode() {
    return glfwGetVideoMode(m_Monitor);
  }

  void Monitor::SetGamma(float gamma) {
    return glfwSetGamma(m_Monitor, gamma);
  }

  const GLFWgammaramp *Monitor::GetGammaRamp() {
    return glfwGetGammaRamp(m_Monitor);
  }

  void Monitor::SetGammaRamp(const GLFWgammaramp *ramp) {
    return glfwSetGammaRamp(m_Monitor, ramp);
  }

  /////////////

  Cursor::Cursor(GLFWimage *img, int xhot, int yhot) : m_Image(img) {
    m_Cursor = glfwCreateCursor(m_Image, xhot, yhot);
  }

  Cursor::~Cursor() {
    glfwDestroyCursor(m_Cursor);
  }

  GLFWcursor *Cursor::CreateStandardCursor(int shape) {
    return glfwCreateStandardCursor(shape);
  }

  void Cursor::Set(Window window) {
    glfwSetCursor(window.Get(), m_Cursor);
  }
} // namespace glfwpp