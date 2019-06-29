#include "format.hpp"

#include <string>
#include <vector>

#include "../config.hpp"

std::string fmt::format(const std::string& fmt, std::vector<std::string> args) {
  std::string res = fmt;
  for (std::size_t i = 0; i < args.size(); ++i) {
    std::string key = "{" + std::to_string(i) + "}";
    std::string::size_type n = 0;
    while ((n = res.find(key, n)) != std::string::npos) {
      res.replace(n, key.size(), args[i]);
      n += args[i].size();
    }
  }
  return res;
}

std::string fmt::get_float(double v, int perc) {
  char buff[255];
  snprintf(buff, 255, "%.*f", perc, v);
  return std::string(buff);
}

#if COLOR_DEPTH == 16
std::string fmt::get_hex(uint64_t c) {
  char buff[255];
  snprintf(buff, 255, "#%012lx", c);
  return std::string(buff);
}
#else
std::string fmt::get_hex(uint32_t c) {
  char buff[255];
  snprintf(buff, 255, "#%06x", c);
  return std::string(buff);
}
#endif
