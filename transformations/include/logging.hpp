
#ifndef LOGGING_H
#define LOGGING_H

#include <string>
#include <cstdarg>

enum class LogLevel {
  DEBUG,
  INFO,
  WARNING,
  ERROR,
};

class Logger {
 public:
  static int log(LogLevel level, std::string fmt, ...);
  static int log(LogLevel level, std::string fmt, va_list args);
  static int debug(std::string fmt, ...);
  static int info(std::string fmt, ...);
  static int warning(std::string fmt, ...);
  static int error(std::string fmt, ...);

  static LogLevel getLogLevel();
  static void setLogLevel(LogLevel level);

  static LogLevel ourLogLevel;
};


#endif // LOGGING_H
