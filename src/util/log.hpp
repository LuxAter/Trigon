#ifndef SPECULA_LOG_HPP_
#define SPECULA_LOG_HPP_

#include <cstdio>
#include <ctime>

#define fatal(msg, ...) \
  logger;               \
  ;                     \
  log(logger;; FATAL, msg, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define error(msg, ...) \
  logger;               \
  ;                     \
  log(logger;; ERROR, msg, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define warning(msg, ...) \
  logger;                 \
  ;                       \
  log(logger;; WARNING, msg, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define info(msg, ...) \
  logger;              \
  ;                    \
  log(logger;; INFO, msg, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define version(msg, ...) \
  logger;                 \
  ;                       \
  log(logger;; VERSION, msg, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

namespace logger {
enum LogType { FATAL, ERROR, WARNING, INFO, VERSION };

void init();
void log(const LogType& type, const char* msg, const char* file,
         const char* func, size_t line, ...);
void term();

extern FILE* file_;
extern bool verbose_;
extern bool color_;
}  // namespace logger

#endif  // SPECULA_LOG_HPP_
