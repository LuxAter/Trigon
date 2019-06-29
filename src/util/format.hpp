#ifndef TRIGON_UTIL_FORMAT_HPP_
#define TRIGON_UTIL_FORMAT_HPP_

#include <string>
#include <vector>

#include "../config.hpp"

namespace fmt {
std::string format(const std::string& fmt, std::vector<std::string> args);
std::string get_float(double v, int perc = 5);
std::string get_hex(COLOR c);
}  // namespace fmt

#endif  // TRIGON_UTIL_FORMAT_HPP_
