#include "terminal.hpp"

#include <iostream>

bool use_color = true;

void error(const std::string& msg) {
  std::cerr << "\033[1;31m[ERROR  ] " << msg << "\033[0m" << std::endl;
}
void warning(const std::string& msg) {
  std::cerr << "\033[1;33m[WARNING] " << msg << "\033[0m" << std::endl;
}
void info(const std::string& msg) {
  std::cout << "\033[1;34m[INFO   ] " << msg << "\033[0m" << std::endl;
}
void success(const std::string& msg) {
  std::cout << "\033[1;32m[SUCCESS] " << msg << "\033[0m" << std::endl;
}

bool verify_error(const std::string& msg) {
  std::cerr << "\033[1;31m" << msg << " [Y/N]\033[0m" << std::endl;
  std::cerr << ">> ";
  char ch;
  std::cin >> ch;
  if (ch == 'y' || ch == 'Y' || ch == 't' || ch == 'T' || ch == '1') {
    return true;
  } else {
    return false;
  }
}
bool verify_warning(const std::string& msg) {
  std::cerr << "\033[1;33m" << msg << " [Y/N]\033[0m" << std::endl;
  std::cerr << ">> ";
  char ch;
  std::cin >> ch;
  if (ch == 'y' || ch == 'Y' || ch == 't' || ch == 'T' || ch == '1') {
    return true;
  } else {
    return false;
  }
}
bool verify_info(const std::string& msg) {
  std::cerr << "\033[1;37m" << msg << " [Y/N]\033[0m" << std::endl;
  std::cerr << ">> ";
  char ch;
  std::cin >> ch;
  if (ch == 'y' || ch == 'Y' || ch == 't' || ch == 'T' || ch == '1') {
    return true;
  } else {
    return false;
  }
}
bool verify_success(const std::string& msg) {
  std::cerr << "\033[1;32m" << msg << " [Y/N]\033[0m" << std::endl;
  std::cerr << ">> ";
  char ch;
  std::cin >> ch;
  if (ch == 'y' || ch == 'Y' || ch == 't' || ch == 'T' || ch == '1') {
    return true;
  } else {
    return false;
  }
}
