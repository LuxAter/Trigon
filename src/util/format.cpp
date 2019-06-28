#include "format.hpp"

#include <string>
#include <vector>

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
