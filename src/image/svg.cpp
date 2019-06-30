#include "svg.hpp"

#include <array>
#include <map>
#include <string>
#include <vector>

#include "../config.hpp"

bool write_svg(
    const std::string& file, const unsigned& w, const unsigned& h,
    std::vector<std::pair<std::string, std::map<std::string, std::string>>>
        elements) {
  FILE* out = fopen(file.c_str(), "w");
  if (!out) {
    return false;
  }
  fprintf(out, "<svg width=\"%u\" height=\"%u\">\n", w, h);
  for (auto& it : elements) {
    std::string line = '<' + it.first + ' ';
    for (auto& kv : it.second) {
      line += kv.first + "=\"" + kv.second + "\" ";
    }
    line += "/>";
    fprintf(out, "%s\n", line.c_str());
  }
  fprintf(out, "</svg>\n");
  fclose(out);
  return true;
}
