#include "opts.hpp"

#include <cstring>
#include <iostream>
#include <vector>

#include "terminal.hpp"
#include "util/format.hpp"

void opts::ArgumentParser::add_option(const std::string& name, std::string help,
                                      Value val, bool positional) {
  std::string long_name = name;
  if (name.size() > 2 && name[1] == ',') {
    long_name = name.substr(2);
    name_map_[name[0]] = long_name;
  }
  options_[long_name] = val;
  if (positional) {
    positional_.push_back(long_name);
  }
}

void opts::ArgumentParser::add_option(const std::string& name, std::string help,
                                      Value::ValueType type, bool positional) {
  std::string long_name = name;
  if (name.size() > 2 && name[1] == ',') {
    long_name = name.substr(2);
    name_map_[name[0]] = long_name;
  }
  options_[long_name] = Value(type);
  if (positional) {
    positional_.push_back(long_name);
  }
}

std::map<std::string, opts::Value> opts::ArgumentParser::parse(int argc,
                                                               char* argv[]) {
  // PARSE OPTIONS
  std::vector<std::string> remaining;
  for (int i = 1; i < argc; ++i) {
    if (strlen(argv[i]) <= 1) {
      continue;
    } else if (argv[i][0] == '-' && argv[i][1] == '-') {
      std::string name(argv[i]), value;
      if (name.find('=') != std::string::npos) {
        value = name.substr(name.find('=') + 1);
        name = name.substr(2, name.find('=') - 2);
      } else {
        name = name.substr(2);
      }
      std::map<std::string, Value>::iterator it;
      if ((it = options_.find(name)) != options_.end()) {
        it->second.count++;
        if (it->second.type_ == Value::ValueType::BOOL) {
          it->second.value = (it->second.value == "0") ? "1" : "0";
        } else {
          if (it->second.implicit_value != std::string()) {
            it->second.value = it->second.implicit_value;
          }
          if (value != std::string()) {
            it->second.value = value;
          } else if (i + 1 < argc) {
            it->second.value = std::string(argv[i + 1]);
            i += 1;
          } else if (it->second.value == std::string()) {
            error(fmt::format("expected an argument for '{0}'", {name}));
          }
        }
      } else {
        warning(fmt::format("unexpected option '{0}'", {name}));
      }
      // LONG NAME
    } else if (argv[i][0] == '-') {
      for (int j = 1; j < strlen(argv[i]); ++j) {
        std::map<char, std::string>::const_iterator it;
        if ((it = name_map_.find(argv[i][j])) != name_map_.end()) {
          options_[it->second].count++;
          if (options_[it->second].type_ == Value::ValueType::BOOL) {
            options_[it->second].value =
                (options_[it->second].value == "0") ? "1" : "0";
          } else {
            if (options_[it->second].implicit_value != std::string()) {
              options_[it->second].value = options_[it->second].implicit_value;
            }
            if (i + 1 < argc && j == strlen(argv[i]) - 1 &&
                argv[i + 1][0] != '-') {
              options_[it->second].value = std::string(argv[i + 1]);
              i += 1;
              break;
            } else if (options_[it->second].value == std::string()) {
              error(
                  fmt::format("expected an argument for '{0}'", {it->second}));
            }
          }
        } else {
          warning(fmt::format("unexpected option '{0}'",
                              {std::string(1, argv[i][j])}));
        }
      }
    } else {
      remaining.push_back(std::string(argv[i]));
    }
  }

  for (auto& it : remaining) {
    bool matched = false;
    for (auto& pos : positional_) {
      if (options_[pos].value == std::string()) {
        options_[pos].count++;
        options_[pos].value = it;
        matched = true;
        break;
      }
    }
    if (!matched) {
      warning(fmt::format("unexpected option '{0}'", {it}));
    }
  }
  return options_;
}
