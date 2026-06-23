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
#include <util/sqlite.hh>
#include <shader/shader.hh>
#include <sqlite3.h>
#include <iostream>
#include <stdexcept>
#include <version.hh>

constexpr std::string_view DB_PATH = DB_NAME;

namespace sqlite {
  Instance::Instance() {
    m_pDB = nullptr;
    if (sqlite3_open(DB_PATH.data(), &m_pDB) != SQLITE_OK) {
      throw std::runtime_error("SQLiteデータベースを開けられません。");
    }
  }

  Instance::~Instance() {
    sqlite3_close(m_pDB);
  }

  bool Instance::Exec(string sql) {
    char *err = nullptr;
    bool suc = sqlite3_exec(m_pDB, sql.c_str(), nullptr, nullptr, &err);
    if (suc != SQLITE_OK) {
      std::cerr << "データ実行に失敗：" << err << std::endl;
      sqlite3_free(err);
      return false;
    }

    return true;
  }

  i64 Instance::GetLastInsertId() {
    return sqlite3_last_insert_rowid(m_pDB);
  }

  ///////////////////

  Stmt::Stmt(sqlite3 *pDB) {
    m_pDB = pDB;
  }

  Stmt::~Stmt() {
    sqlite3_finalize(m_pSTMT);
  }

  bool Stmt::Prepare(const std::string &sql) {
    if (sqlite3_prepare_v2(m_pDB, sql.c_str(), static_cast<int>(sql.size() + 1), &m_pSTMT, nullptr) != SQLITE_OK) {
      throw std::runtime_error("SQLの準備に失敗。");
    }

    return true;
  }


  bool Stmt::BindBlob(int index, any data, u32 size) {
    return sqlite3_bind_blob(m_pSTMT, index, data, size, SQLITE_TRANSIENT);
  }

  bool Stmt::BindBlob64(int index, any data, u64 size) {
    return sqlite3_bind_blob64(m_pSTMT, index, data, size, SQLITE_TRANSIENT);
  }

  bool Stmt::BindDouble(int index, f64 data) {
    return sqlite3_bind_double(m_pSTMT, index, data);
  }

  bool Stmt::BindInt(int index, i32 data) {
    return sqlite3_bind_int(m_pSTMT, index, data);
  }

  bool Stmt::BindInt64(int index, i64 data) {
    return sqlite3_bind_int64(m_pSTMT, index, data);
  }

  bool Stmt::BindNull(int index) {
    return sqlite3_bind_null(m_pSTMT, index);
  }
  
  bool Stmt::BindText(int index, const string &data) {
    if (data.empty()) {
      return sqlite3_bind_text(m_pSTMT, index, "", 0, SQLITE_TRANSIENT) == SQLITE_OK;
    }
    return sqlite3_bind_text(m_pSTMT, index,
                             data.c_str(),
                             static_cast<int>(data.size()),
                             SQLITE_TRANSIENT) == SQLITE_OK;
  }

  bool Stmt::BindText16(int index, const string &data) {
    return sqlite3_bind_text16(m_pSTMT, index, data.c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK;
  }

  bool Stmt::BindText64(int index, const string &data) {
    return sqlite3_bind_text64(m_pSTMT, index, data.c_str(), data.size(), SQLITE_TRANSIENT, SQLITE_UTF8) == SQLITE_OK;
  }

  const void *Stmt::ColumnBlob(int col) {
    return sqlite3_column_blob(m_pSTMT, col);
  }

  f64 Stmt::ColumnDouble(int col) {
    return sqlite3_column_double(m_pSTMT, col);
  }

  int Stmt::ColumnInt(int col) {
    return sqlite3_column_int(m_pSTMT, col);;
  }

  i64 Stmt::ColumnInt64(int col) {
    return sqlite3_column_int64(m_pSTMT, col);
  }

  string Stmt::ColumnText(int col) {
    const unsigned char *text = sqlite3_column_text(m_pSTMT, col);
    if (text == nullptr) return "";
    int len = sqlite3_column_bytes(m_pSTMT, col);
    return std::string(reinterpret_cast<const char *>(text), len);
  }

  //const any Stmt::ColumnText16(int col) {
  //  return sqlite3_column_text16(m_pSTMT, col);
  //}

  sqlite3_value *Stmt::ColumnValue(int col) {
    return sqlite3_column_value(m_pSTMT, col);
  }

  int Stmt::ColumnBytes(int col) {
    return sqlite3_column_bytes(m_pSTMT, col);
  }

  int Stmt::ColumnBytes16(int col) {
    return sqlite3_column_bytes16(m_pSTMT, col);
  }

  int Stmt::ColumnType(int col) {
    return sqlite3_column_type(m_pSTMT, col);
  }


  int Stmt::Step() {
    return sqlite3_step(m_pSTMT);
  }
} // namespace sqlite