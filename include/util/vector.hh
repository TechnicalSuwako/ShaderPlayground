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
#ifndef VECTOR_HH
#define VECTOR_HH

#include <util/types.hh>

union Vector2 {
  struct Position {
    f32 x;
    f32 y;
  };
  struct UV {
    f32 u;
    f32 v;
  };

  Position pos;
  UV uv;
};

union Vector2f {
  struct Position {
    f32 x;
    f32 y;
  };
  struct UV {
    f32 u;
    f32 v;
  };

  Position pos;
  UV uv;
};

union Vector2d {
  struct Position {
    f64 x;
    f64 y;
  };
  struct UV {
    f64 u;
    f64 v;
  };

  Position pos;
  UV uv;
};

union Vector2i {
  struct Position {
    i32 x;
    i32 y;
  };
  struct UV {
    i32 u;
    i32 v;
  };

  Position pos;
  UV uv;
};

union Vector2u {
  struct Position {
    u32 x;
    u32 y;
  };
  struct UV {
    u32 u;
    u32 v;
  };

  Position pos;
  UV uv;
};

union Vector3 {
  struct Position {
    f32 x;
    f32 y;
    f32 z;
  };
  struct RGB {
    f32 r;
    f32 g;
    f32 b;
  };

  Position pos;
  RGB rgb;
};

union Vector3f {
  struct Position {
    f32 x;
    f32 y;
    f32 z;
  };
  struct RGB {
    f32 r;
    f32 g;
    f32 b;
  };

  Position pos;
  RGB rgb;
};

union Vector3d {
  struct Position {
    f64 x;
    f64 y;
    f64 z;
  };
  struct RGB {
    f64 r;
    f64 g;
    f64 b;
  };

  Position pos;
  RGB rgb;
};

union Vector3i {
  struct Position {
    i32 x;
    i32 y;
    i32 z;
  };
  struct RGB {
    i32 r;
    i32 g;
    i32 b;
  };

  Position pos;
  RGB rgb;
};

union Vector3u {
  struct Position {
    u32 x;
    u32 y;
    u32 z;
  };
  struct RGB {
    u32 r;
    u32 g;
    u32 b;
  };

  Position pos;
  RGB rgb;
};

union Vector4 {
  struct Position {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
  };
  struct RGBA {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
  };

  Position pos;
  RGBA rgba;
};

union Vector4f {
  struct Position {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
  };
  struct RGBA {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
  };

  Position pos;
  RGBA rgba;
};

union Vector4d {
  struct Position {
    f64 x;
    f64 y;
    f64 z;
    f64 w;
  };
  struct RGBA {
    f64 r;
    f64 g;
    f64 b;
    f64 a;
  };

  Position pos;
  RGBA rgba;
};

union Vector4i {
  struct Position {
    i32 x;
    i32 y;
    i32 z;
    i32 w;
  };
  struct RGBA {
    i32 r;
    i32 g;
    i32 b;
    i32 a;
  };

  Position pos;
  RGBA rgba;
};

union Vector4u {
  struct Position {
    u32 x;
    u32 y;
    u32 z;
    u32 w;
  };
  struct RGBA {
    u32 r;
    u32 g;
    u32 b;
    u32 a;
  };

  Position pos;
  RGBA rgba;
};

inline Vector3 vec3Sub(Vector3 a, Vector3 b) {
  return { a.pos.x - b.pos.x, a.pos.y - b.pos.y, a.pos.z - b.pos.z };
}

inline f32 vec3Dot(Vector3 a, Vector3 b) {
  return a.pos.x * b.pos.x + a.pos.y * b.pos.y + a.pos.z * b.pos.z;
}

inline Vector3 vec3Cross(Vector3 a, Vector3 b) {
  return {
    a.pos.y * b.pos.z - a.pos.z * b.pos.y,
    a.pos.z * b.pos.x - a.pos.x * b.pos.z,
    a.pos.x * b.pos.y - a.pos.y * b.pos.x,
  };
}

inline Vector3 vec3Normalize(Vector3 v) {
  f32 len = sqrtf(v.pos.x * v.pos.x + v.pos.y * v.pos.y + v.pos.z * v.pos.z);
  if (len == 0.f) return { 0, 0, 0 };
  return { v.pos.x / len, v.pos.y / len, v.pos.z / len };
}

#endif // VECTOR_HH