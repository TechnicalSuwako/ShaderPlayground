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
#include <database/locale.hh>
#include <util/sqlitepp.hh>
#include <sqlite3.h>
#include <iostream>

namespace db {
  Locale::Locale(sqlitepp::Instance *db) : m_DB(db) {
    {
      sqlitepp::Stmt stmt(db->GetDB());
      stmt.Prepare("SELECT id, name, code FROM languages ORDER BY id ASC;");

      while (stmt.Step() == SQLITE_ROW) {
        Languages lang;
        lang.id = stmt.ColumnInt(0);
        lang.name = stmt.ColumnText(1);
        lang.code = stmt.ColumnText(2);

        sqlitepp::Stmt langStmt(db->GetDB());
        langStmt.Prepare("SELECT key, value FROM locale WHERE language_id = ?;");
        langStmt.BindInt(1, lang.id);

        while (langStmt.Step() == SQLITE_ROW)
          lang.dict.emplace(langStmt.ColumnText(0), langStmt.ColumnText(1));

        m_Langs.emplace_back(std::move(lang));
      }
    }

    setLang();
  }

  string Locale::GetWord(const string &key) const {
    auto it = m_CurLang.dict.find(key);
    return it != m_CurLang.dict.end() ? it->second : key;
  }

  void Locale::SetLanguage(i32 id) {
    sqlitepp::Stmt stmt(m_DB->GetDB());
    stmt.Prepare("UPDATE settings SET value = ? WHERE key = ?;");
    stmt.BindInt(1, id);
    stmt.BindText(2, "locale");

    if (stmt.Step() != SQLITE_DONE) {
      std::cerr << "言語変更に失敗。" << std::endl;
      return;
    }
    setLang();
  }

  //////////

  void Locale::setLang() noexcept {
    sqlitepp::Stmt stmt(m_DB->GetDB());
    stmt.Prepare("SELECT value FROM settings WHERE key = ?;");
    stmt.BindText(1, "locale");

    if ((stmt.Step() == SQLITE_ROW)) {
      i32 curLangId = stmt.ColumnInt(0);
      for (const auto &lang : m_Langs) {
        if (lang.id == curLangId) {
          m_CurLang = lang;
          break;
        }
      }
    }
  }
} // namespace db