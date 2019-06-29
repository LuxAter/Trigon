#include "image.hpp"

#include <cstdlib>

#include <algorithm>

#include "../util/format.hpp"

image::Image::Image(const uint64_t& w, const uint64_t& h)
    : width_(w), height_(h) {
  if (height_ == 0) {
    height_ = width_;
  }
}

image::Image::~Image() {}

void image::Image::fill(uint32_t color) {
  elements.push_back(
      {RECT,
       {{"width", "100%"}, {"height", "100%"}, {"fill", fmt::get_hex(color)}}});
}

void image::Image::rect(double x, double y, double w, double h, double rx,
                        double ry, uint32_t fill, uint32_t stroke_width,
                        uint32_t stroke) {
  elements.push_back({RECT,
                      {{"x", fmt::get_float(x)},
                       {"y", fmt::get_float(y)},
                       {"width", fmt::get_float(w)},
                       {"height", fmt::get_float(h)},
                       {"rx", fmt::get_float(rx)},
                       {"ry", fmt::get_float(ry)},
                       {"stroke", fmt::get_hex(stroke)},
                       {"stroke-width", std::to_string(stroke_width)},
                       {"fill", fmt::get_hex(fill)}}});
}

void image::Image::circle(double r, double cx, double cy, uint32_t fill,
                          uint32_t stroke_width, uint32_t stroke) {
  elements.push_back({CIRCLE,
                      {{"r", fmt::get_float(r)},
                       {"cx", fmt::get_float(cx)},
                       {"cy", fmt::get_float(cy)},
                       {"stroke", fmt::get_hex(stroke)},
                       {"stroke-width", std::to_string(stroke_width)},
                       {"fill", fmt::get_hex(fill)}}});
}

void image::Image::ellipse(double rx, double ry, double cx, double cy,
                           uint32_t fill, uint32_t stroke_width,
                           uint32_t stroke) {
  elements.push_back({ELLIPSE,
                      {{"rx", fmt::get_float(rx)},
                       {"ry", fmt::get_float(ry)},
                       {"cx", fmt::get_float(cx)},
                       {"cy", fmt::get_float(cy)},
                       {"stroke", fmt::get_hex(stroke)},
                       {"stroke-width", std::to_string(stroke_width)},
                       {"fill", fmt::get_hex(fill)}}});
}

void image::Image::line(double x1, double y1, double x2, double y2,
                        uint32_t fill, uint32_t stroke_width, uint32_t stroke) {
  elements.push_back({ELLIPSE,
                      {{"x1", fmt::get_float(x1)},
                       {"y1", fmt::get_float(y1)},
                       {"x2", fmt::get_float(x2)},
                       {"y1", fmt::get_float(y2)},
                       {"stroke", fmt::get_hex(stroke)},
                       {"stroke-width", std::to_string(stroke_width)},
                       {"fill", fmt::get_hex(fill)}}});
}

void image::Image::polyline(std::vector<std::array<double, 2>> points,
                            uint32_t fill, uint32_t stroke_width,
                            uint32_t stroke) {
  std::string str_points = "";
  for (auto& it : points) {
    str_points += fmt::get_float(it[0]) + ' ' + fmt::get_float(it[1]) + ',';
  }
  str_points.pop_back();
  elements.push_back({POLYLINE,
                      {{"points", str_points},
                       {"stroke", fmt::get_hex(stroke)},
                       {"stroke-width", std::to_string(stroke_width)},
                       {"fill", fmt::get_hex(fill)}}});
}
void image::Image::polyline(std::vector<double> x, std::vector<double> y,
                            uint32_t fill, uint32_t stroke_width,
                            uint32_t stroke) {
  std::string str_points = "";
  for (std::size_t i = 0; i < x.size() && i < y.size(); ++i) {
    str_points += fmt::get_float(x[i]) + ' ' + fmt::get_float(y[i]) + ',';
  }
  str_points.pop_back();
  elements.push_back({POLYLINE,
                      {{"points", str_points},
                       {"stroke", fmt::get_hex(stroke)},
                       {"stroke-width", std::to_string(stroke_width)},
                       {"fill", fmt::get_hex(fill)}}});
}

void image::Image::polygon(std::vector<std::array<double, 2>> points,
                           uint32_t fill, uint32_t stroke_width,
                           uint32_t stroke) {
  std::string str_points = "";
  for (auto& it : points) {
    str_points += fmt::get_float(it[0]) + ' ' + fmt::get_float(it[1]) + ',';
  }
  str_points.pop_back();
  elements.push_back({POLYGON,
                      {{"points", str_points},
                       {"stroke", fmt::get_hex(stroke)},
                       {"stroke-width", std::to_string(stroke_width)},
                       {"fill", fmt::get_hex(fill)}}});
}
void image::Image::polygon(std::vector<double> x, std::vector<double> y,
                           uint32_t fill, uint32_t stroke_width,
                           uint32_t stroke) {
  std::string str_points = "";
  for (std::size_t i = 0; i < x.size() && i < y.size(); ++i) {
    str_points += fmt::get_float(x[i]) + ' ' + fmt::get_float(y[i]) + ',';
  }
  str_points.pop_back();
  elements.push_back({POLYGON,
                      {{"points", str_points},
                       {"stroke", fmt::get_hex(stroke)},
                       {"stroke-width", std::to_string(stroke_width)},
                       {"fill", fmt::get_hex(fill)}}});
}

void image::Image::triangle(double x1, double y1, double x2, double y2,
                            double x3, double y3, uint32_t fill,
                            uint32_t stroke_width, uint32_t stroke) {
  elements.push_back(
      {POLYGON,
       {{"points", fmt::get_float(x1) + ' ' + fmt::get_float(y1) + ',' +
                       fmt::get_float(x2) + ' ' + fmt::get_float(y2) + ',' +
                       fmt::get_float(x3) + ' ' + fmt::get_float(y3)},
        {"stroke", fmt::get_hex(stroke)},
        {"stroke-width", std::to_string(stroke_width)},
        {"fill", fmt::get_hex(fill)}}});
}
