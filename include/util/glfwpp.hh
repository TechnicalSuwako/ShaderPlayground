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
#ifndef GLFWPP_HH
#define GLFWPP_HH

#include <glfw/glfw3.h>
#include "types.hh"
#include <span>

namespace glfwpp {
  enum ErrorCodes {
    NotInitialized     = 0x00010001,
    NoCurrentContext   = 0x00010002,
    InvalidEnum        = 0x00010003,
    InvalidValue       = 0x00010004,
    OutOfMemory        = 0x00010005,
    ApiUnavailable     = 0x00010006,
    VersionUnavailable = 0x00010007,
    PlatformError      = 0x00010008,
    FormatUnavailable  = 0x00010009,
    NoWindowContent    = 0x0001000A,
  };

  enum Attributes {
    Focused                 = 0x00020001,
    Iconified               = 0x00020002,
    Resizable               = 0x00020003,
    visible                 = 0x00020004,
    Decorated               = 0x00020005,
    AutoNotify              = 0x00020006,
    Floating                = 0x00020007,
    Maximized               = 0x00020008,

    RedBits                 = 0x00021001,
    GreenBits               = 0x00021002,
    BlueBits                = 0x00021003,
    AlphaBits               = 0x00021004,
    DepthBits               = 0x00021005,
    StencilBits             = 0x00021006,
    AccumRedBits            = 0x00021007,
    AccumGreenBits          = 0x00021008,
    AccumBlueBits           = 0x00021009,
    AccumAlphaBits          = 0x0002100A,
    AuxBuffers              = 0x0002100B,
    Stereo                  = 0x0002100C,
    Samples                 = 0x0002100D,
    SrgbCapable             = 0x0002100E,
    RefreshRate             = 0x0002100F,
    Doublebuffer            = 0x00021010,

    ClientApi               = 0x00022001,
    ContextVersionMajor     = 0x00022002,
    ContextVersionMinor     = 0x00022003,
    ContextRevision         = 0x00022004,
    ContextRobustness       = 0x00022005,
    OpenGLForwardCompatable = 0x00022006,
    OpenGLDebugContext      = 0x00022007,
    OpenGLProfile           = 0x00022008,
    ContextReleaseBehavior  = 0x00022009,
    ContextNoError          = 0x0002200A,

    NoApi                   = 0,
    OpenGLApi               = 0x00030001,
    OpenGLESApi             = 0x00030002,

    NoRobustness            = 0,
    NoResetNotification     = 0x00031001,
    LoseContextOnReset      = 0x00031002,

    AnyOpenGLProfile        = 0,
    OpenGLCore              = 0x00032001,
    OpenGLCompat            = 0x00032002,

    Cursor                  = 0x00033001,
    StickyKeys              = 0x00033002,
    StickyMouseButtons      = 0x00033003,

    CursorNormal            = 0x00034001,
    CursorHidden            = 0x00034002,
    CursorDisabled          = 0x00034003,

    AnyReleaseBehavior      = 0,
    ReleaseBehaviorFlush    = 0x00035001,
    ReleaseBehaviorNone     = 0x00035002,
  };

  enum CursorShapes {
    ArrowCursor     = 0x00036001,
    IbeamCursor     = 0x00036002,
    CrosshairCursor = 0x00036003,
    HandCursor      = 0x00036004,
    HResizeCursor   = 0x00036005,
    VResizeCursor   = 0x00036006,
  };

  enum Connection {
    Connected    = 0x00040001,
    Disconnected = 0x00040002,
    DontCare     = -1,
  };

  enum ButtonMode {
    LockKeyMods = 0x00032004,
    RawMouseMotion = 0x00032005,
  };

  enum ContextApi {
    Native = 0x00032001,
    EGL = 0x00032002,
    OSMESA = 0x00032003,
  };

  class Instance {
    public:
      Instance();
      ~Instance();

      GLFWerrorfun SetErrorCallback();

      void SetContextMajorVersion(int major);
      void SetContextMinorVersion(int minor);
      void SetOpenGLProfile(int profile);
      void SetForwardCompatibility(bool fc);
      void SetMaximized(bool fc);

      void CreateWindowHints();
      void PollEvents();

      void SwapInterval(int interval);
      double GetTime();
      void SetTime(double time);
      u64 GetTimerValue();
      u64 GetTimerFrequency();

      void InitHint(int hint, int value);
      void GetVersion(int *major, int *minor, int *rev);
      cstr GetVersionString(void);

      int GetError(cstr *description);
      GLFWerrorfun SetErrorCallback(GLFWerrorfun cbfun);

    private:
      int m_Major = 0;
      int m_Minor = 0;
      int m_Profile = 0;
      bool m_ForwardCompat = false;
      bool m_IsMaximized = false;
  }; // class Instance

  class Window {
    public:
      Window(int width, int height, string title);
      ~Window();

      GLFWwindow *Get() const { return m_Window; }
      GLFWframebuffersizefun SetFramebufferSizeCallback();

      bool ShouldClose() const;
      void SwapBuffers();

      void GetSize(int *w, int *h);
      void Maximize();
      void SetClipboardString(string str);
      string GetClipboardString();
      void MakeContextCurrent();
      int GetAttrib(int attrib);
      int GetInputMode(int mode);
      void SetInputMode(int mode, int value);
      int RawMouseMotionSupported();
      cstr GetKeyName(int key, int scancode);
      int GetKeyScancode(int key);
      int GetKey(int key);
      int GetMouseButton(int button);
      void GetCursorPos(double *xpos, double *ypos);
      void SetCursorPos(double xpos, double ypos);

      GLFWwindow *GetCurrentContext();
      bool IsExtensionSupported(cstr ext);
      GLFWglproc GetProcAddress(cstr procname);

      GLFWkeyfun SetKeyCallback(GLFWkeyfun cbfun);
      GLFWcharfun SetCharCallback(GLFWcharfun cbfun);
      GLFWcharmodsfun SetCharModsCallback(GLFWcharmodsfun cbfun);
      GLFWmousebuttonfun SetMouseButtonCallback(GLFWmousebuttonfun cbfun);
      GLFWcursorposfun SetCursorPosCallback(GLFWcursorposfun cbfun);
      GLFWcursorenterfun SetCursorEnterCallback(GLFWcursorenterfun cbfun);
      GLFWscrollfun SetScrollCallback(GLFWscrollfun cbfun);
      GLFWdropfun SetDropCallback(GLFWdropfun cbfun);

      int JoystickPresent(int jid);
      const f32 *GetJoystickAxes(int jid, int *count);
      const unsigned char *GetJoystickButtons(int jid, int *count);
      const unsigned char *GetJoystickHats(int jid, int *count);
      cstr GetJoystickName(int jid);
      cstr GetJoystickGUID(int jid);
      void SetJoystickUserPointer(int jid, any ptr);
      any GetJoystickUserPointer(int jid);
      GLFWjoystickfun SetJoystickCallback(GLFWjoystickfun cbfun);
      int UpdateGamepadMappings(string str);
      int JoystickIsGamepad(int jid);
      cstr GetGamepadName(int jid);
      int GetGamepadState(int jid, GLFWgamepadstate *state);

    private:
      GLFWwindow *m_Window = {};
      int m_Width = 0;
      int m_Height = 0;
  }; // class Window

  class Monitor {
    public:
      Monitor();
      ~Monitor();

      GLFWmonitor *Get() const { return m_Monitor; }
      std::span<GLFWmonitor *> GetMonitors(int *count);
      GLFWmonitor *GetPrimary();
      void GetPos(int *xpos, int *ypos);
      void GetWorkarea(int *xpos, int *ypos, int *width, int *height);
      void GetPhysicalSize(int *width, int *height);
      void GetContentScale(float *xscale, float *yscale);
      cstr GetName();
      void SetUserPointer(any ptr);
      any GetUserPointer();
      GLFWmonitorfun SetCallback(GLFWmonitorfun cbfun);
      const GLFWvidmode *GetVideoModes(int *count);
      const GLFWvidmode *GetVideoMode();
      void SetGamma(float gamma);
      const GLFWgammaramp *GetGammaRamp();
      void SetGammaRamp(const GLFWgammaramp *ramp);

    private:
      GLFWmonitor *m_Monitor = {};
  }; // class Monitor

  class Cursor {
    public:
      Cursor(GLFWimage *img, int xhot, int yhot);
      ~Cursor();

      GLFWcursor *Get() const { return m_Cursor; }
      GLFWcursor *CreateStandardCursor(int shape);
      void Set(Window window);

    private:
      GLFWimage *m_Image;
      GLFWcursor *m_Cursor;
  }; // class Cursor
} // namespace glfwpp

#endif // GLFWPP_HH