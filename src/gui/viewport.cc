/*
# 076 License

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
#include <glad/glad.h>
#include <gui/viewport.hh>
#include <imgui.h>

namespace gui {
  void ViewPort::Init() {
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGB,
      m_Width,
      m_Height,
      0,
      GL_RGB,
      GL_UNSIGNED_BYTE,
      nullptr
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(
      GL_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT0,
      GL_TEXTURE_2D,
      m_Texture,
      0
    );

    glGenRenderbuffers(1, &m_RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(
      GL_RENDERBUFFER,
      GL_DEPTH24_STENCIL8,
      m_Width,
      m_Height
    );

    glFramebufferRenderbuffer(
      GL_FRAMEBUFFER,
      GL_DEPTH_STENCIL_ATTACHMENT,
      GL_RENDERBUFFER,
      m_RBO
    );

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void ViewPort::Resize(i32 width, i32 height) {
    m_Width = width;
    m_Height = height;

    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGB,
      width,
      height,
      0,
      GL_RGB,
      GL_UNSIGNED_BYTE,
      nullptr
    );

    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(
      GL_RENDERBUFFER,
      GL_DEPTH24_STENCIL8,
      width,
      height
    );
  }

  void ViewPort::Draw(GlfwInfo &glfw) {
    string tit = glfw.i18n->GetWord("editorviewport") + "###View";
    ImGui::Begin(tit.c_str());

    ImVec2 avail = ImGui::GetContentRegionAvail();
    if ((i32)avail.x != m_Width || (i32)avail.y != m_Height) {
      Resize((i32)avail.x, (i32)avail.y);
    }

    ImGui::Image((ImTextureID)(intptr_t)m_Texture, avail, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
  }
} // namespace gui