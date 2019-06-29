#include "svg.hpp"

#include <cstdio>
#include <string>

#include "image.hpp"

std::string image::get_tag(const SVGTag& tag) {
  switch (tag) {
    case CIRCLE:
      return "circle";
    case RECT:
      return "rect";
    case ELLIPSE:
      return "ellipse";
    case LINE:
      return "line";
    case POLYLINE:
      return "polyline";
    case POLYGON:
      return "polygon";
    case PATH:
      return "path";
  }
}

bool image::write_svg(const std::string& file_path, const Image& img) {
  FILE* out = fopen(file_path.c_str(), "w");
  if (!out) return false;
  fprintf(out, "<svg width=\"%lu\" height=\"%lu\">\n", img.width_, img.height_);
  for (auto& el : img.elements) {
    std::string line = "<" + get_tag(el.first) + ' ';
    for (auto& kv : el.second) {
      line += kv.first + "=\"" + kv.second + "\" ";
    }
    line += "/>";
    fprintf(out, "%s\n", line.c_str());
  }
  fprintf(out, "</svg>\n");
  fclose(out);
  return true;
}
