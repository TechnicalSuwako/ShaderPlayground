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
#ifndef GLFW_HH
#define GLFW_HH

#include <glfw/glfw3.h>
#include "types.hh"

namespace glfw {
  enum class Context {
    VersionMajor = 0x00022002,
    VersionMinor = 0x00022003,
  };

  enum class Api {
    None = 0,
    OpenGL = 0x00032001,
    OpenGLES = 0x00032002,
  };

  enum class Robustness {
    None = 0,
    NoResetNotification = 0x00032001,
    LoseContextOnReset = 0x00032002,
  };

  enum OpenGLProfile {
    Any = 0,
    Core = 0x00032001,
    Compat = 0x00032002,
  };

  enum class ButtonMode {
    Cursor = 0x00032001,
    StickyKeys = 0x00032002,
    StickyMouseButtons = 0x00032003,
    LockKeyMods = 0x00032004,
    RawMouseMotion = 0x00032005,
  };

  enum class Cursor {
    Normal = 0x00032001,
    Hidden = 0x00032002,
    Disabled = 0x00032003,
  };

  enum class ReleaseBehavior {
    Any = 0,
    Flush = 0x00032001,
    None = 0x00032002,
  };

  enum class ContextApi {
    Native = 0x00032001,
    EGL = 0x00032002,
    OSMESA = 0x00032003,
  };

  class Instance {
    public:
      Instance();
      ~Instance();

      void SetContextMajorVersion(int major);
      void SetContextMinorVersion(int minor);
      void SetOpenGLProfile(int profile);
      void SetForwardCompatibility(bool fc);
      void CreateWindowHints();

    private:
      int m_Major = 0;
      int m_Minor = 0;
      int m_Profile = 0;
      bool m_ForwardCompat = false;
  }; // class Instance

  class Window {
    public:
      Window(int width, int height, string title);
      ~Window();

      GLFWwindow *GetWindow() const { return window; }
      GLFWframebuffersizefun SetFramebufferSizeCallback();

      bool ShouldClose() const;
      void SwapBuffers();
      void PollEvents();

      void SetClipboardString(string str);
      string GetClipboardString();
      double GetTime();
      void SetTime(double time);
      u64 GetTimerValue();
      u64 GetTimerFrequency();

    private:
      GLFWwindow *window = {};
      int width = 0;
      int height = 0;
  }; // class Window
} // namespace glfw

#endif