#pragma once

#include <util/types.hh>

namespace gui {
  enum class LogType {
    Normal,
    Info,
    Warning,
    Error,
  };

  struct LogEntry {
    LogType type;
    string text;
  };

  class ConsoleLog {
    public:
      void Add(LogEntry entry);
      void Draw(cstr title = "コンソール");

    private:
      void Clear();

    private:
      vector<LogEntry> m_Lines;
  };
} // namespace gui