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
#ifndef VECTOR_HH
#define VECTOR_HH

#include "types.hh"

typedef union {
  struct {
    f32 x;
    f32 y;
  };
  struct {
    f32 u;
    f32 v;
  };
} Vector2;

typedef union {
  struct {
    i32 x;
    i32 y;
  };
  struct {
    i32 u;
    i32 v;
  };
} Vector2i;

typedef union {
  struct {
    u32 x;
    u32 y;
  };
  struct {
    u32 u;
    u32 v;
  };
} Vector2u;

typedef union {
  struct {
    f32 x;
    f32 y;
    f32 z;
  };
  struct {
    f32 r;
    f32 g;
    f32 b;
  };
} Vector3;

typedef union {
  struct {
    i32 x;
    i32 y;
    i32 z;
  };
  struct {
    i32 r;
    i32 g;
    i32 b;
  };
} Vector3i;

typedef union {
  struct {
    u32 x;
    u32 y;
    u32 z;
  };
  struct {
    u32 r;
    u32 g;
    u32 b;
  };
} Vector3u;

typedef union {
  struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
  };
  struct {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
  };
} Vector4;

typedef union {
  struct {
    i32 x;
    i32 y;
    i32 z;
    i32 w;
  };
  struct {
    i32 r;
    i32 g;
    i32 b;
    i32 a;
  };
} Vector4i;

typedef union {
  struct {
    u32 x;
    u32 y;
    u32 z;
    u32 w;
  };
  struct {
    u32 r;
    u32 g;
    u32 b;
    u32 a;
  };
} Vector4u;

#endif