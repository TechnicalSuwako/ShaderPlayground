/*
# 076 License

Copyright (c) 076 Studio LLC

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
#include <util/sqlitepp.hh>
#include <sqlite3.h>
#include <version.hh>
#include <cassert>
#include <iostream>

namespace db {
  void setDefaultSetting(sqlitepp::Instance &db, const string &key, i32 value) {
    sqlitepp::Stmt stmt(db.GetDB());
    stmt.Prepare("INSERT INTO settings (key, value) VALUES (?, ?);");
    stmt.BindText(1, key);
    stmt.BindInt(2, value);
    assert(stmt.Step() == SQLITE_DONE && "最初のシェーダーを入れ込みに失敗。\n");
  }

  void createLanguage(sqlitepp::Instance &db, const string &name, const string &code) {
    sqlitepp::Stmt stmt(db.GetDB());
    stmt.Prepare("INSERT INTO languages (name, code) VALUES (?, ?);");
    stmt.BindText(1, name);
    stmt.BindText(2, code);
    assert(stmt.Step() == SQLITE_DONE && "最初のシェーダーを入れ込みに失敗。\n");
  }

  void createWord(sqlitepp::Instance &db, i32 langId, const string &key, const string &value = "") {
    sqlitepp::Stmt stmt(db.GetDB());
    stmt.Prepare("INSERT INTO locale (language_id, key, value) VALUES (?, ?, ?);");
    stmt.BindInt(1, langId);
    stmt.BindText(2, key);
    stmt.BindText(3, value);
    assert(stmt.Step() == SQLITE_DONE && "最初のシェーダーを入れ込みに失敗。\n");
  }

  void Initialize(sqlitepp::Instance &db) {
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

CREATE TABLE IF NOT EXISTS scenes (
  id          INTEGER PRIMARY KEY,
  name        TEXT UNIQUE NOT NULL,
  description TEXT,
  created_at  DATETIME DEFAULT CURRENT_TIMESTAMP,
  updated_at  DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE UNIQUE INDEX IF NOT EXISTS idx_scenes_name ON scenes(name);

CREATE TRIGGER IF NOT EXISTS scenes_updated_at
AFTER UPDATE ON scenes
BEGIN
  UPDATE scenes
  SET updated_at = CURRENT_TIMESTAMP
  WHERE id = NEW.id;
END;

CREATE TABLE IF NOT EXISTS scene_skybox (
  id          INTEGER PRIMARY KEY,
  scene_id    INTEGER NOT NULL,
  clear_r     REAL NOT NULL,
  clear_g     REAL NOT NULL,
  clear_b     REAL NOT NULL,
  clear_a     REAL NOT NULL,
  created_at  DATETIME DEFAULT CURRENT_TIMESTAMP,
  updated_at  DATETIME DEFAULT CURRENT_TIMESTAMP,

  FOREIGN KEY (scene_id) REFERENCES scenes(id) ON DELETE CASCADE
);

CREATE TRIGGER IF NOT EXISTS scene_skybox_updated_at
AFTER UPDATE ON scene_skybox
BEGIN
  UPDATE scene_skybox
  SET updated_at = CURRENT_TIMESTAMP
  WHERE id = NEW.id;
END;

CREATE TABLE IF NOT EXISTS scene_code (
  id          INTEGER PRIMARY KEY,
  scene_id    INTEGER NOT NULL,
  code_type   INTEGER NOT NULL,
  code        TEXT NOT NULL,
  filename    TEXT,
  created_at  DATETIME DEFAULT CURRENT_TIMESTAMP,
  updated_at  DATETIME DEFAULT CURRENT_TIMESTAMP,

  FOREIGN KEY (scene_id) REFERENCES scenes(id) ON DELETE CASCADE
);

CREATE TRIGGER IF NOT EXISTS scene_code_updated_at
AFTER UPDATE ON scene_code
BEGIN
  UPDATE scene_code
  SET updated_at = CURRENT_TIMESTAMP
  WHERE id = NEW.id;
END;

CREATE TABLE IF NOT EXISTS languages (
  id          INTEGER PRIMARY KEY,
  name        TEXT NOT NULL,
  code        TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS locale (
  id          INTEGER PRIMARY KEY,
  language_id INTEGER,
  key         TEXT NOT NULL,
  value       TEXT,

  FOREIGN KEY (language_id) REFERENCES languages(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS settings (
  id          INTEGER PRIMARY KEY,
  key         TEXT NOT NULL,
  value       INTEGER
);)";
    
    db.Exec(sql);

    {
      sqlitepp::Stmt stmt(db.GetDB());
      stmt.Prepare("SELECT COUNT(*) FROM version;");
      if (stmt.Step() == SQLITE_ROW) {
        int count = stmt.ColumnInt(0);
        if (count > 0) return;
      }
    }

    {
      sqlitepp::Stmt stmt(db.GetDB());
      stmt.Prepare("INSERT INTO version (major, minor, revision) VALUES (?, ?, ?);");
      stmt.BindInt(1, version::major);
      stmt.BindInt(2, version::minor);
      stmt.BindInt(3, version::revision);

      assert(stmt.Step() == SQLITE_DONE && "最初のシェーダーを入れ込みに失敗。\n");
    }

    {
      setDefaultSetting(db, "locale", 1);
    }

    {
      createLanguage(db, "日本語", "ja_JP");
      createLanguage(db, "English", "en_US");
    }

    {
      createWord(db, 1, "file", "ファイル(F)");
      createWord(db, 2, "file", "File");
      createWord(db, 1, "filenew", "新しいファイル(N)...");
      createWord(db, 2, "filenew", "New File...");
      createWord(db, 1, "filenewcreateshader", "新しいシェーダーを作成");
      createWord(db, 2, "filenewcreateshader", "New Shader Creation");
      createWord(db, 1, "filenew2dshader", "2D シェーダー");
      createWord(db, 2, "filenew2dshader", "2D Shader");
      createWord(db, 1, "filenew3dshader", "3D シェーダー");
      createWord(db, 2, "filenew3dshader", "3D Shader");
      createWord(db, 1, "filenewsceneshader", "シーン シェーダー");
      createWord(db, 2, "filenewsceneshader", "Scene Shader");
      createWord(db, 1, "fileopen", "開く(O)...");
      createWord(db, 2, "fileopen", "Open...");
      createWord(db, 1, "fileopenshader", "シェーダーの開く");
      createWord(db, 2, "fileopenshader", "Open Shader");
      createWord(db, 1, "filesave", "保存(S)");
      createWord(db, 2, "filesave", "Save");
      createWord(db, 1, "filesaveas", "名前を付けて保存(A)...");
      createWord(db, 2, "filesaveas", "Save as...");
      createWord(db, 1, "filesaveasshader", "シェーダーの保存");
      createWord(db, 2, "filesaveasshader", "Save Shader");
      createWord(db, 1, "fileshadername", "シェーダー名");
      createWord(db, 2, "fileshadername", "Shader Name");
      createWord(db, 1, "quit", "終了(X)");
      createWord(db, 2, "quit", "Quit");
      createWord(db, 1, "cancel", "キャンセル(C)");
      createWord(db, 2, "cancel", "Cancel");
      createWord(db, 1, "ok", "OK(O)");
      createWord(db, 2, "ok", "OK");
      createWord(db, 1, "editorvertshader", "頂点シェーダー");
      createWord(db, 2, "editorvertshader", "Vertex Shader");
      createWord(db, 1, "editorfragshader", "フラグメントシェーダー");
      createWord(db, 2, "editorfragshader", "Fragment Shader");
      createWord(db, 1, "editorlua", "Luaエディター");
      createWord(db, 2, "editorlua", "Lua Editor");
      createWord(db, 1, "editorviewport", "ビューポート");
      createWord(db, 2, "editorviewport", "Viewport");
      createWord(db, 1, "editorconsole", "コンソール");
      createWord(db, 2, "editorconsole", "Console");
      createWord(db, 1, "editorconsoleclear", "消す(X)");
      createWord(db, 2, "editorconsoleclear", "Clear");
      createWord(db, 1, "edit", "編集(E)");
      createWord(db, 2, "edit", "Edit");
      createWord(db, 1, "editundo", "元に戻す(U)");
      createWord(db, 2, "editundo", "Undo");
      createWord(db, 1, "editredo", "やり直し(R)");
      createWord(db, 2, "editredo", "Redo");
      createWord(db, 1, "editcut", "切り取り(C)");
      createWord(db, 2, "editcut", "Cut");
      createWord(db, 1, "editcopy", "コピー(Y)");
      createWord(db, 2, "editcopy", "Copy");
      createWord(db, 1, "editpaste", "貼り付け(P)");
      createWord(db, 2, "editpaste", "Paste");
      createWord(db, 1, "editduplicate", "行を複製(D)");
      createWord(db, 2, "editduplicate", "Duplicate");
      createWord(db, 1, "settings", "設定(S)");
      createWord(db, 2, "settings", "Settings");
      createWord(db, 1, "build", "ビルド(B)");
      createWord(db, 2, "build", "Build");
      createWord(db, 1, "buildcompile", "コンパイル(C)");
      createWord(db, 2, "buildcompile", "Compile");
      createWord(db, 1, "help", "ヘルプ(H)");
      createWord(db, 2, "help", "Help");
      createWord(db, 1, "helpmanual", "マニュアル(M)");
      createWord(db, 2, "helpmanual", "Manual");
      createWord(db, 1, "helpmanualoverview", "概要");
      createWord(db, 2, "helpmanualoverview", "Overview");
      createWord(db, 1, "helpmanualshader", "シェーダー");
      createWord(db, 2, "helpmanualshader", "Shader");
      createWord(db, 1, "helpmanuallua", "Luaエンジン");
      createWord(db, 2, "helpmanuallua", "Lua Engine");
      createWord(db, 1, "helpmanualhotkey", "ホットキー");
      createWord(db, 2, "helpmanualhotkey", "Hotkeys");
      createWord(db, 1, "helpabout", "Shader Playgroundについて(A)");
      createWord(db, 2, "helpabout", "About Shader Playground");
      createWord(db, 1, "currentfps", "現在フレームレート");
      createWord(db, 2, "currentfps", "Current Frame Rate");
      createWord(db, 1, "consolelogerrorluainvalidcomp", "Luaコードが不正ですので、コンパイル出来ませんでした。");
      createWord(db, 2, "consolelogerrorluainvalidcomp", "Your Lua code is invalid, so it could not be compiled.");
      createWord(db, 1, "consolelogerrorluainvalidsave", "Luaコードが不正ですので、保存出来ませんでした。");
      createWord(db, 2, "consolelogerrorluainvalidsave", "Your Lua code is invalid, so it could not be saved.");
      createWord(db, 1, "consoleloginfocompileok", "シェーダーをコンパイルしました。");
      createWord(db, 2, "consoleloginfocompileok", "Shaders got compiled successfully.");
      createWord(db, 1, "consoleloginfosaveok", "シェーダー及びLuaを保存しました。");
      createWord(db, 2, "consoleloginfosaveok", "Shaders and Lua have been saved.");
      createWord(db, 1, "consoleloginfolangchangeok", "言語を変更しました。");
      createWord(db, 2, "consoleloginfolangchangeok", "Language has been changed.");
      createWord(db, 1, "consoleloginfocreateok", "新しいシェーダーを作成しました。保存するには、「CTRL + S」を押して下さい。");
      createWord(db, 2, "consoleloginfocreateok", "New shader has been created. In order to save, press \"CTRL + S\".");
      createWord(db, 1, "clearcolor", "クリアカラー");
      createWord(db, 2, "clearcolor", "Clear color");
    }

    i64 shaderId = 0;

    {
      sqlitepp::Stmt stmt(db.GetDB());
      stmt.Prepare("INSERT INTO scenes (name, description) VALUES (?, ?);");
      stmt.BindText(1, "default");
      stmt.BindText(2, "");
      assert(stmt.Step() == SQLITE_DONE && "最初のシェーダーを入れ込みに失敗。\n");
      shaderId = db.GetLastInsertId();
    }

    {
      sqlitepp::Stmt stmt(db.GetDB());
      stmt.Prepare("INSERT INTO scene_skybox (scene_id, clear_r, clear_g, clear_b, clear_a) VALUES (?, ?, ?, ?, ?);");
      stmt.BindInt(1, shaderId);
      stmt.BindDouble(2, 0.f);
      stmt.BindDouble(3, 0.f);
      stmt.BindDouble(4, 0.f);
      stmt.BindDouble(5, 1.f);

      assert(stmt.Step() == SQLITE_DONE && "最初のシェーダーを入れ込みに失敗。\n");
    }

    {
      sqlitepp::Stmt stmt(db.GetDB());
      stmt.Prepare("INSERT INTO scene_code (scene_id, code_type, code, filename) VALUES (?, ?, ?, ?);");
      stmt.BindInt(1, shaderId);
      stmt.BindInt(2, static_cast<int>(db::ShaderCodeType::GlslVertex));
      stmt.BindText(3, GetDefaultVertexShader());
      stmt.BindText(4, "default.vert");

      assert(stmt.Step() == SQLITE_DONE && "最初のシェーダーを入れ込みに失敗。\n");
    }

    {
      sqlitepp::Stmt stmt(db.GetDB());
      stmt.Prepare("INSERT INTO scene_code (scene_id, code_type, code, filename) VALUES (?, ?, ?, ?);");
      stmt.BindInt(1, shaderId);
      stmt.BindInt(2, static_cast<int>(db::ShaderCodeType::GlslFragment));
      stmt.BindText(3, GetDefaultFragmentShader());
      stmt.BindText(4, "default.frag");

      assert(stmt.Step() == SQLITE_DONE && "最初のシェーダーを入れ込みに失敗。\n");
    }

    {
      sqlitepp::Stmt stmt(db.GetDB());
      stmt.Prepare("INSERT INTO scene_code (scene_id, code_type, code, filename) VALUES (?, ?, ?, ?);");
      stmt.BindInt(1, shaderId);
      stmt.BindInt(2, static_cast<int>(db::ShaderCodeType::Lua));
      stmt.BindText(3, GetDefaultLuaCode());
      stmt.BindText(4, "default.lua");

      assert(stmt.Step() == SQLITE_DONE && "最初のシェーダーを入れ込みに失敗。\n");
    }
  }
} // namespace db