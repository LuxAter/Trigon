#ifndef SPECULA_LOG_HPP_
#define SPECULA_LOG_HPP_

#include <cstdio>
#include <ctime>

#define fatal(msg, ...)                                                     \
  log::log(log::FATAL, msg, __FILE__, __func__, __LINE__, \
                    ##__VA_ARGS__)
#define error(msg, ...)                                                     \
  log::log(log::ERROR, msg, __FILE__, __func__, __LINE__, \
                    ##__VA_ARGS__)
#define warning(msg, ...)                                                     \
  log::log(log::WARNING, msg, __FILE__, __func__, __LINE__, \
                    ##__VA_ARGS__)
#define info(msg, ...)                                                     \
  log::log(log::INFO, msg, __FILE__, __func__, __LINE__, \
                    ##__VA_ARGS__)
#define version(msg, ...)                                                     \
  log::log(log::VERSION, msg, __FILE__, __func__, __LINE__, \
                    ##__VA_ARGS__)

namespace log {
  enum LogType { FATAL, ERROR, WARNING, INFO, VERSION };

  void init();
  void log(const LogType& type, const char* msg, const char* file,
           const char* func, size_t line, ...);
  void term();

  extern FILE* file_;
  extern bool verbose_;
  extern bool color_;
}  // namespace log

#endif  // SPECULA_LOG_HPP_
