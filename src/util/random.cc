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
#include <util/random.hh>
#include <cmath>
#include <chrono>

namespace util::rand {
  u32 randState[4];

  static inline u32 leftRot(const u32 x, i32 k) {
    return (x << k) | (x >> (32 - k));
  }

  u32 xoshiro128() {
    const u32 res = leftRot(randState[1] * 5, 7) * 9;
    const u32 t = randState[1] << 9;

    randState[2] ^= randState[0];
    randState[3] ^= randState[1];
    randState[1] ^= randState[2];
    randState[0] ^= randState[3];

    randState[2] ^= t;
    randState[3] = leftRot(randState[3], 11);

    return res;
  }

  i32 getValue(i32 min, i32 max) {
    u32 range = static_cast<u32>(max - min + 1);
    return min + (xoshiro128() % range);
  }

  static u32 splitmix32(u32 &x) {
    u32 z = (x += 0x9e3779b9);
    z = (z ^ (z >> 16)) * 0x85ebca6b;
    z = (z ^ (z >> 13)) * 0xc2b2ae35;
    return z ^ (z >> 16);
  }
  
  //////////////////////

  void Seed(u32 seed) {
    for (int i = 0; i < 4; i++) randState[i] = splitmix32(seed);
  }
  
  i32 GetRandom(i32 min, i32 max) {
    if (min > max) {
      i32 tmp = max;
      max = min;
      min = tmp;
    }

    return getValue(min, max);
  }
} // namespace util::rand