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
#ifndef SQLITEPP_HH
#define SQLITEPP_HH

#include <util/types.hh>

struct sqlite3;
struct sqlite3_stmt;
struct sqlite3_value;

namespace sqlitepp {
  class Instance {
    public:
      Instance();
      ~Instance();

      sqlite3 *GetDB() { return m_pDB; }
      bool Exec(string sql);
      i64 GetLastInsertId();

    private:
      sqlite3 *m_pDB;
  }; // class Instance

  class Stmt {
    public:
      Stmt(sqlite3 *pDB);
      ~Stmt();

      bool Prepare(const std::string &sql);
      bool BindBlob(int index, any data, u32 size);
      bool BindBlob64(int index, any data, u64 size);
      bool BindDouble(int index, f64 data);
      bool BindInt(int index, i32 data);
      bool BindInt64(int index, i64 data);
      bool BindNull(int index);
      bool BindText(int index, const string &data);
      bool BindText16(int index, const string &data);
      bool BindText64(int index, const string &data);

      const void *ColumnBlob(int col);
      f64 ColumnDouble(int col);
      int ColumnInt(int col);
      i64 ColumnInt64(int col);
      string ColumnText(int col);
      //const any ColumnText16(int col);
      sqlite3_value *ColumnValue(int col);
      int ColumnBytes(int col);
      int ColumnBytes16(int col);
      int ColumnType(int col);
      int Step();

    private:
      sqlite3 *m_pDB;
      sqlite3_stmt *m_pSTMT = nullptr;
  }; // class Stmt
} // namespace sqlitepp

#endif // SQLITEPP_HH