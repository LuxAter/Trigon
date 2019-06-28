#ifndef TRIGON_UTIL_FORMAT_HPP_
#define TRIGON_UTIL_FORMAT_HPP_

#include <string>
#include <vector>

namespace fmt {
std::string format(const std::string& fmt, std::vector<std::string> args);
}  // namespace fmt

#endif  // TRIGON_UTIL_FORMAT_HPP_
