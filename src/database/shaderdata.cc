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
#include <database/shaderdata.hh>
#include <util/sqlitepp.hh>
#include <sqlite3.h>
#include <iostream>
#include <cassert>

namespace db {
  void getShaderCode(sqlitepp::Instance &db, ShaderData &data) {
    sqlitepp::Stmt stmt(db.GetDB());
    stmt.Prepare("SELECT id, code_type, code, filename FROM shader_code WHERE shader_id = ?;");
    stmt.BindInt(1, data.id);

    while (stmt.Step() == SQLITE_ROW) {
      u32 id = stmt.ColumnInt(0);
      ShaderCodeType type = static_cast<ShaderCodeType>(stmt.ColumnInt(1));
      string code = stmt.ColumnText(2);
      string filename = stmt.ColumnText(3);

      if (type == ShaderCodeType::GlslVertex) {
        data.vertexShader.id = id;
        data.vertexShader.type = type;
        data.vertexShader.code = code;
        data.vertexShader.filename = filename;
      } else if (type == ShaderCodeType::GlslFragment) {
        data.fragmentShader.id = id;
        data.fragmentShader.type = type;
        data.fragmentShader.code = code;
        data.fragmentShader.filename = filename;
      } else if (type == ShaderCodeType::Lua) {
        data.luaCode.id = id;
        data.luaCode.type = type;
        data.luaCode.code = code;
        data.luaCode.filename = filename;
      } else {
        throw std::exception("未対応コード類。");
      }
    }
  }

  vector<ShaderData> GetAllShaders(sqlitepp::Instance &db) {
    vector<ShaderData> out;

    sqlitepp::Stmt stmt(db.GetDB());
    stmt.Prepare("SELECT id, name, description FROM shaders ORDER BY updated_at DESC;");

    while (stmt.Step() == SQLITE_ROW) {
      ShaderData data;
      data.id = stmt.ColumnInt(0);
      data.name = stmt.ColumnText(1);
      data.description = stmt.ColumnText(2);
      getShaderCode(db, data);

      out.push_back(std::move(data));
    }

    return out;
  }

  ShaderData GetShader(sqlitepp::Instance &db, u32 id) {
    ShaderData data = {};
    sqlitepp::Stmt stmt(db.GetDB());
    stmt.Prepare("SELECT id, name, description FROM shaders WHERE id = ?;");
    stmt.BindInt(1, id);

    while (stmt.Step() == SQLITE_ROW) {
      data.id = stmt.ColumnInt(0);
      data.name = stmt.ColumnText(1);
      data.description = stmt.ColumnText(2);
      getShaderCode(db, data);
    }

    return data;
  }

  void insertShaderCode(sqlitepp::Instance &db, const CodeData &data, u32 id) {
    sqlitepp::Stmt stmt(db.GetDB());
    stmt.Prepare("INSERT INTO shader_code (shader_id, code_type, code, filename) VALUES (?, ?, ?, ?);");
    stmt.BindInt(1, id);
    stmt.BindInt(2, static_cast<u32>(data.type));
    stmt.BindText(3, data.code);
    stmt.BindText(4, data.filename);
    assert(stmt.Step() == SQLITE_DONE && "シェーダーコードの保存に失敗。\n");
  }

  void SaveNewShader(sqlitepp::Instance &db, const ShaderData &data) {
    sqlitepp::Stmt stmt(db.GetDB());
    stmt.Prepare("INSERT INTO shaders (name, description) VALUES (?, ?);");

    stmt.BindText(1, data.name);
    stmt.BindText(2, data.description);

    assert(stmt.Step() == SQLITE_DONE && "シェーダーの保存に失敗。\n");
    u32 id = GetLastShaderID(db);
    insertShaderCode(db, data.vertexShader, id);
    insertShaderCode(db, data.fragmentShader, id);
    insertShaderCode(db, data.luaCode, id);
  }

  void SaveCode(sqlitepp::Instance &db, const CodeData &data) {
    sqlitepp::Stmt stmt(db.GetDB());
    stmt.Prepare("UPDATE shader_code SET code = ?, filename = ? WHERE id = ?;");

    stmt.BindText(1, data.code);
    stmt.BindText(2, data.filename);
    stmt.BindInt(3, data.id);

    assert(stmt.Step() == SQLITE_DONE && "コードの保存に失敗。\n");
  }

  u32 GetLastShaderID(sqlitepp::Instance &db) {
    sqlitepp::Stmt stmt(db.GetDB());
    stmt.Prepare("SELECT MAX(id) FROM shaders;");
    return (stmt.Step() == SQLITE_ROW) ? stmt.ColumnInt(0) : 0;
  }
} // namespace db