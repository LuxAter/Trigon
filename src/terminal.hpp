#ifndef TRIGON_TERMINAL_HPP_
#define TRIGON_TERMINAL_HPP_

#include <string>

extern bool use_color;

void error(const std::string& msg);
void warning(const std::string& msg);
void info(const std::string& msg);
void success(const std::string& msg);

bool verify_error(const std::string& msg);
bool verify_warning(const std::string& msg);
bool verify_info(const std::string& msg);
bool verify_success(const std::string& msg);

#endif  // TRIGON_TERMINAL_HPP_
