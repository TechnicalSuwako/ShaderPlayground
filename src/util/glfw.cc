/*
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
#include <util/glfw.hh>
#include <iostream>

namespace glfw {
  Instance::Instance() {
    if (!glfwInit()) {
      throw "GLFWの初期化に失敗しました";
    }
  }

  Instance::~Instance() {
    glfwTerminate();
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

  void Instance::CreateWindowHints() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_Major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_Minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, m_Profile);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, m_ForwardCompat);
  }

  /////////////

  Window::Window(int width, int height, string title) {
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
      throw "GLFWウィンドウの作成に失敗しました";
    }

    glfwMakeContextCurrent(window);
  }

  Window::~Window() {
    glfwDestroyWindow(window);
  }

  void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
  }

  GLFWframebuffersizefun Window::SetFramebufferSizeCallback() {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return nullptr;
  }

  bool Window::ShouldClose() const {
    return glfwWindowShouldClose(window);
  }

  void Window::SwapBuffers() {
    glfwSwapBuffers(window);
  }
  void Window::PollEvents() {
    glfwPollEvents();
  }

  void Window::SetClipboardString(string str) {
    glfwSetClipboardString(window, str.c_str());
  }

  string Window::GetClipboardString() {
    return string(glfwGetClipboardString(window));
  }

  double Window::GetTime() {
    return glfwGetTime();
  }

  void Window::SetTime(double time) {
    return glfwSetTime(time);
  }

  u64 Window::GetTimerValue() {
    return glfwGetTimerValue();
  }

  u64 Window::GetTimerFrequency() {
    return glfwGetTimerFrequency();
  }
} // namespace glfw