#include "argparse.hpp"

#include <map>
#include <string>
#include <variant>
#include <vector>

#include <iostream>

namespace argparse {
std::vector<Option> options_;
}  // namespace argparse

void argparse::add_argument(char short_name, std::string long_name,
                            std::string help) {
  options_.push_back({short_name, long_name, help});
  options_.back().val = false;
  options_.back().type = BOOL;
}
void argparse::add_argument(char short_name, std::string long_name, bool val,
                            std::string help) {
  options_.push_back({short_name, long_name, help});
  options_.back().val = val;
  options_.back().type = BOOL;
}
void argparse::add_argument(char short_name, std::string long_name,
                            const char* val, std::string help) {
  options_.push_back({short_name, long_name, help});
  options_.back().val = std::string(val);
  options_.back().type = STRING;
}
void argparse::add_argument(char short_name, std::string long_name,
                            std::string val, std::string help) {
  options_.push_back({short_name, long_name, help});
  options_.back().val = val;
  options_.back().type = STRING;
}
void argparse::add_argument(char short_name, std::string long_name, int val,
                            std::string help) {
  options_.push_back({short_name, long_name, help});
  options_.back().val = val;
  options_.back().type = INT;
}
void argparse::add_argument(char short_name, std::string long_name, double val,
                            std::string help) {
  options_.push_back({short_name, long_name, help});
  options_.back().val = val;
  options_.back().type = DOUBLE;
}
std::map<std::string, argparse::Variable> argparse::parse(int argc,
                                                          char** argv) {
  std::vector<std::string> args;
  for (int i = 1; i < argc; ++i) {
    std::string carg(argv[i]);
    if (carg.find('=') != std::string::npos) {
      if (carg[1] == '-') {
        args.push_back(carg.substr(0, carg.find('=')));
      } else {
        for (std::size_t j = 1; j < carg.find('='); ++j) {
          args.push_back(std::string("-") + carg[j]);
        }
      }
      args.push_back(carg.substr(carg.find('=') + 1));
    } else if (carg[1] == '-') {
      args.push_back(carg);
    } else if (carg[0] == '-') {
      for (std::size_t j = 1; j < carg.size(); ++j) {
        args.push_back(std::string("-") + carg[j]);
      }
    } else {
      args.push_back(carg);
    }
  }
  std::map<std::string, Variable> opts;
  for (auto& opt : options_) {
    opts[opt.long_name] = Variable{opt.type, opt.val};
  }
  for (std::size_t i = 0; i < args.size(); ++i) {
    if ((args[i][1] == 'h' && args[i].size() == 2) || (args[i] == "--help")) {
      print_help(argv[0]);
    }
    for (auto& opt : options_) {
      if ((args[i][1] == opt.short_name && args[i].size() == 2) ||
          args[i] == "--" + opt.long_name) {
        if (opt.type == BOOL) {
          opts[opt.long_name].val = static_cast<bool>(!std::get<bool>(opt.val));
        } else if (args.size() > i + 1) {
          if (opt.type == STRING) {
            opts[opt.long_name].val = args[i + 1];
          } else if (opt.type == INT) {
            opts[opt.long_name].val = std::stoi(args[i + 1]);
          } else if (opt.type == DOUBLE) {
            opts[opt.long_name].val = std::stod(args[i + 1]);
          }
          i++;
        } else {
          printf("Argument %s requires an argument!\n", args[i].c_str());
          exit(-1);
        }
        break;
      }
    }
  }
  return opts;
}

void argparse::print_help(const std::string& cmd) {
  printf("Usage: %s\n", cmd.c_str());
  for (auto& opt : options_) {
    switch (opt.type) {
      case BOOL:
        printf("  -%c --%s  %s\n", opt.short_name, opt.long_name.c_str(),
               opt.help.c_str());
        break;
      case STRING:
        printf("  -%c --%s[%s]  %s\n", opt.short_name, opt.long_name.c_str(),
               std::get<std::string>(opt.val).c_str(), opt.help.c_str());
        break;
      case INT:
        printf("  -%c --%s[%d]  %s\n", opt.short_name, opt.long_name.c_str(),
               std::get<int>(opt.val), opt.help.c_str());
        break;
      case DOUBLE:
        printf("  -%c --%s[%f]  %s\n", opt.short_name, opt.long_name.c_str(),
               std::get<double>(opt.val), opt.help.c_str());
        break;
    }
  }
  exit(0);
}
