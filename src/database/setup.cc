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
#include <database/setup.hh>
#include <shader/shader.hh>
#include <util/sqlite.hh>
#include <sqlite3.h>
#include <version.hh>
#include <cassert>
#include <iostream>

namespace db {
  void Initialize(sqlite::Instance &db) {
    cstr sql = R"(CREATE TABLE IF NOT EXISTS version (
  id          INTEGER PRIMARY KEY,
  major       INTEGER NOT NULL,
  minor       INTEGER NOT NULL,
  revision    INTEGER NOT NULL,
  created_at  DATETIME DEFAULT CURRENT_TIMESTAMP,
  updated_at  DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TRIGGER IF NOT EXISTS version_updated_at
AFTER UPDATE ON version
BEGIN
  UPDATE version 
  SET updated_at = CURRENT_TIMESTAMP 
  WHERE id = NEW.id;
END;

CREATE TABLE IF NOT EXISTS shaders (
  id          INTEGER PRIMARY KEY,
  name        TEXT UNIQUE NOT NULL,
  description TEXT,
  created_at  DATETIME DEFAULT CURRENT_TIMESTAMP,
  updated_at  DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE UNIQUE INDEX IF NOT EXISTS idx_shaders_name ON shaders(name);

CREATE TRIGGER IF NOT EXISTS shaders_updated_at
AFTER UPDATE ON shaders
BEGIN
  UPDATE shaders 
  SET updated_at = CURRENT_TIMESTAMP 
  WHERE id = NEW.id;
END;

CREATE TABLE IF NOT EXISTS shader_code (
  id          INTEGER PRIMARY KEY,
  shader_id   INTEGER NOT NULL,
  code_type   INTEGER NOT NULL,
  code        TEXT NOT NULL,
  filename    TEXT,
  created_at  DATETIME DEFAULT CURRENT_TIMESTAMP,
  updated_at  DATETIME DEFAULT CURRENT_TIMESTAMP,

  FOREIGN KEY (shader_id) REFERENCES shaders(id) ON DELETE CASCADE
);

CREATE TRIGGER IF NOT EXISTS shader_code_updated_at
AFTER UPDATE ON shader_code
BEGIN
  UPDATE shader_code
  SET updated_at = CURRENT_TIMESTAMP 
  WHERE id = NEW.id;
END;)";
    
    db.Exec(sql);

    {
      sqlite::Stmt stmt(db.GetDB());
      stmt.Prepare("SELECT COUNT(*) FROM version;");
      if (stmt.Step() == SQLITE_ROW) {
        int count = stmt.ColumnInt(0);
        if (count > 0) return;
      }
    }

    {
      sqlite::Stmt stmt(db.GetDB());
      stmt.Prepare("INSERT INTO version (major, minor, revision) VALUES (?, ?, ?);");
      stmt.BindInt(1, version::major);
      stmt.BindInt(2, version::minor);
      stmt.BindInt(3, version::revision);

      assert(stmt.Step() == SQLITE_DONE && "最初のシェーダーを入れ込みに失敗。\n");
    }

    i64 shaderId = 0;

    {
      sqlite::Stmt stmt(db.GetDB());
      stmt.Prepare("INSERT INTO shaders (name, description) VALUES (?, ?);");
      stmt.BindText(1, "default");
      stmt.BindText(2, "");
      assert(stmt.Step() == SQLITE_DONE && "最初のシェーダーを入れ込みに失敗。\n");
      shaderId = db.GetLastInsertId();
    }

    {
      sqlite::Stmt stmt(db.GetDB());
      stmt.Prepare("INSERT INTO shader_code (shader_id, code_type, code, filename) VALUES (?, ?, ?, ?);");
      stmt.BindInt(1, shaderId);
      stmt.BindInt(2, static_cast<int>(db::ShaderCodeType::GlslVertex));
      stmt.BindText(3, GetDefaultVertexShader());
      stmt.BindText(4, "default.vert");

      assert(stmt.Step() == SQLITE_DONE && "最初のシェーダーを入れ込みに失敗。\n");
    }

    {
      sqlite::Stmt stmt(db.GetDB());
      stmt.Prepare("INSERT INTO shader_code (shader_id, code_type, code, filename) VALUES (?, ?, ?, ?);");
      stmt.BindInt(1, shaderId);
      stmt.BindInt(2, static_cast<int>(db::ShaderCodeType::GlslFragment));
      stmt.BindText(3, GetDefaultFragmentShader());
      stmt.BindText(4, "default.frag");

      assert(stmt.Step() == SQLITE_DONE && "最初のシェーダーを入れ込みに失敗。\n");
    }

    {
      sqlite::Stmt stmt(db.GetDB());
      stmt.Prepare("INSERT INTO shader_code (shader_id, code_type, code, filename) VALUES (?, ?, ?, ?);");
      stmt.BindInt(1, shaderId);
      stmt.BindInt(2, static_cast<int>(db::ShaderCodeType::Lua));
      stmt.BindText(3, GetDefaultLuaCode());
      stmt.BindText(4, "default.lua");

      assert(stmt.Step() == SQLITE_DONE && "最初のシェーダーを入れ込みに失敗。\n");
    }
  }
} // namespace db