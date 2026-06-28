/*
# 076 License

Copyright (c) 076 Studio LLC

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
#pragma once

#include <util/glfwpp.hh>
#include <util/vector.hh>
#include <version.hh>
#include <functional>
#include <string>
#include <cstdint>
#include <database/shaderdata.hh>
#include <TextEditor.h>

namespace gui {
  class ConsoleLog;
} // namespace gui

namespace db {
  class Locale;
  struct CodeData;
} // namespace db

struct Info {
  glfwpp::Instance *instance;
  glfwpp::Window *window;

  bool isRunning = true;
  bool isAbout = false;
  bool isManual = false;
  bool isSettings = false;
  bool isLangChange = false;

  bool showNewShaderPopup = false;
  bool showOpenShaderPopup = false;
  bool showSaveAsShaderPopup = false;
  bool viewportFocused = false;
  bool editorFocused = false;
  bool pendingCompile = false;

  std::string version = version::full;
  uint32_t shaderId = 0;
  std::string shaderName = "";
  db::CodeData VERT = {};
  db::CodeData FRAG = {};
  db::CodeData LUA = {};
  std::string currentFile = "";
  Vector4 sceneClearColor = {};
  vector<TextEditor *> textEditors;
  TextEditor *activeEditor;

  gui::ConsoleLog *cmd;
  db::Locale *i18n;

  std::function<void()> compile;
  std::function<void()> save;
  std::function<void()> create;
  std::function<void()> open;
  std::function<void()> saveAs;
};