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
#include <util/file.hh>

#include <iostream>
#include <fstream>

vector<u8> read_file(const string &filename, bool binary) {
  std::ios::openmode flags;
  if (binary) {
    flags = std::ios::binary | std::ios::ate;
  } else {
    flags = std::ios::in;
  }

  std::ifstream file(filename, flags);
  if (!file) {
    throw std::runtime_error("ファイルを開くに失敗： " + filename);
  }

  const auto size = file.tellg();
  file.seekg(0);
  vector<u8> data(static_cast<size_t>(size));
  if (!file.read(reinterpret_cast<char *>(data.data()), size)) {
    throw std::runtime_error("ファイルの読み込みに失敗。");
  }

  file.close();

  return data;
}

bool write_file(const string &filename, vector<string> contents, bool binary) {
  std::ios::openmode flags;
  if (binary) {
    flags = std::ios::binary | std::ios::ate;
  } else {
    flags = std::ios::out | std::ios::trunc;
  }

  std::ofstream file(filename, flags);
  if (!file) {
    throw std::runtime_error("ファイルを開くに失敗： " + filename);
  }

  for (const auto &c : contents) {
    file << c;
    file << "\n";
  }

  file.close();

  return true;
}