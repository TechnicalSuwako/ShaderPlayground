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
#ifndef MATRIX_HH
#define MATRIX_HH

#include <util/types.hh>
#include <util/vector.hh>

typedef struct {
  f32 m[16];
} Matrix4;

typedef struct {
  f32 m[16];
} Matrix4x4;

typedef struct {
  f32 m[12];
} Matrix4x3;

inline Matrix4 operator*(const Matrix4 &a, const Matrix4 &b) {
  Matrix4 out = {};

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      float sum = 0.0f;
      for (int k = 0; k < 4; ++k) {
        sum += a.m[i + k * 4] * b.m[k + j * 4];
      }
      out.m[i + j * 4] = sum;
    }
  }
  return out;
}

inline Matrix4 &operator*=(Matrix4 &a, const Matrix4 &b) {
  a = a * b;
  return a;
}

inline Matrix4 mat4Identity() {
  Matrix4 out{};

  out.m[0] = 1.f;
  out.m[5] = 1.f;
  out.m[10] = 1.f;
  out.m[15] = 1.f;

  return out;
}

inline Matrix4x4 mat4x4Identity() {
  Matrix4x4 out{};

  out.m[0] = 1.f;
  out.m[5] = 1.f;
  out.m[10] = 1.f;
  out.m[15] = 1.f;

  return out;
}

inline Matrix4 mat4Mul(const Matrix4 &a, const Matrix4 &b) {
  Matrix4 out{};

  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      out.m[col * 4 + row] =
        a.m[0 * 4 + row] * b.m[col * 4 + 0] +
        a.m[1 * 4 + row] * b.m[col * 4 + 1] +
        a.m[2 * 4 + row] * b.m[col * 4 + 2] +
        a.m[3 * 4 + row] * b.m[col * 4 + 3];
    }
  }

  return out;
}

inline Matrix4 mat4Translate(f32 x, f32 y, f32 z) {
  Matrix4 out = mat4Identity();

  out.m[12] = x;
  out.m[13] = y;
  out.m[14] = z;

  return out;
}

inline Matrix4 mat4Scale(f32 x, f32 y, f32 z) {
  Matrix4 out = {};

  out.m[0] = x;
  out.m[5] = y;
  out.m[10] = z;
  out.m[15] = 1.f;

  return out;
}

inline Matrix4 mat4RotateX(f32 angle) {
  Matrix4 out = mat4Identity();

  f32 c = cosf(angle);
  f32 s = sinf(angle);

  out.m[5]  =  c; // m[1][1]
  out.m[6]  =  s; // m[1][2]
  out.m[9]  = -s; // m[2][1]
  out.m[10] =  c; // m[2][2]

  return out;
}

inline Matrix4 mat4RotateY(f32 angle) {
  Matrix4 out = mat4Identity();

  f32 c = cosf(angle);
  f32 s = sinf(angle);

  out.m[0]  =  c; // m[0][0]
  out.m[2]  = -s; // m[0][2]
  out.m[8]  =  s; // m[2][0]
  out.m[10] =  c; // m[2][2]

  return out;
}

inline Matrix4 mat4RotateZ(f32 angle) {
  Matrix4 out = mat4Identity();

  f32 c = cosf(angle);
  f32 s = sinf(angle);

  out.m[0] =  c; // m[0][0]
  out.m[1] =  s; // m[0][1]
  out.m[4] = -s; // m[1][0]
  out.m[5] =  c; // m[1][1]

  return out;
}

inline Matrix4 mat4Rotate(f32 pitch, f32 yaw, f32 roll) {
  return mat4RotateZ(roll) * mat4RotateY(yaw) * mat4RotateX(pitch);
}

inline Matrix4 mat4Perspective(f32 fov, f32 aspect, f32 near, f32 far) {
  Matrix4 out = {};
  fov *= (3.14159265f / 180.f);

  f32 tanHalf = tanf(fov * 0.5f);

  out.m[0] = 1.f / (aspect * tanHalf);
  out.m[5] = 1.f / tanHalf;
  out.m[10] = -(far + near) / (far - near);
  out.m[11] = -1.f;
  out.m[14] = -(2.f * far * near) / (far - near);

  return out;
}

inline Matrix4 mat4LookAt(Vector3 eye, Vector3 target, Vector3 up) {
  Vector3 f = vec3Normalize(vec3Sub(target, eye));
  Vector3 r = vec3Normalize(vec3Cross(f, up));
  Vector3 u = vec3Cross(r, f);

  Matrix4 out = {};

  out.m[0] = r.pos.x;
  out.m[1] = u.pos.x;
  out.m[2] = -f.pos.x;
  out.m[3] = 0.f;

  out.m[4] = r.pos.y;
  out.m[5] = u.pos.y;
  out.m[6] = -f.pos.y;
  out.m[7] = 0.f;

  out.m[8] = r.pos.z;
  out.m[9] = u.pos.z;
  out.m[10] = -f.pos.z;
  out.m[11] = 0.f;

  out.m[12] = -vec3Dot(r, eye);
  out.m[13] = -vec3Dot(u, eye);
  out.m[14] = vec3Dot(f, eye);
  out.m[15] = 1.f;

  return out;
}

#endif // MATRIX_HH