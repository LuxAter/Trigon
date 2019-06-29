#ifndef TRIGON_UTIL_STRING_HPP_
#define TRIGON_UTIL_STRING_HPP_

#include <string>

inline bool endswith(const std::string& base, const std::string& suffix) {
  return !base.compare(base.size() - suffix.size(), suffix.size(), suffix);
}
inline bool startswith(const std::string& base, const std::string& prefix) {
  return !base.compare(0, prefix.size(), prefix);
}

#endif  // TRIGON_UTIL_STRING_HPP_
