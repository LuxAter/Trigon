#ifndef TRIGON_IMG_HPP_
#define TRIGON_IMG_HPP_

#include <cstdint>
#include <map>
#include <vector>

namespace image {
static std::map<uint32_t, uint32_t> resolutions_{{1920, 1080}};

enum SVGTag { CIRCLE, RECT, ELLIPSE, LINE, POLYLINE, POLYGON, PATH };

class Image {
 public:
  Image(const uint64_t& w, const uint64_t& h);
  ~Image();

  void fill(uint32_t color);

  void rect(double x, double y, double w, double h, double rx = 0,
            double ry = 0, uint32_t fill = 0x000000, uint32_t stroke_width = 0,
            uint32_t stroke = 0x000000);
  void circle(double r, double cx, double cy, uint32_t fill = 0x000000,
              uint32_t stroke_width = 0, uint32_t stroke = 0x000000);
  void ellipse(double rx, double ry, double cx, double cy,
               uint32_t fill = 0x000000, uint32_t stroke_width = 0,
               uint32_t stroke = 0x000000);
  void line(double x1, double y1, double x2, double y2,
            uint32_t fill = 0x000000, uint32_t stroke_width = 0,
            uint32_t stroke = 0x000000);
  void polyline(std::vector<std::array<double, 2>> points,
                uint32_t fill = 0x000000, uint32_t stroke_width = 0,
                uint32_t stroke = 0x000000);
  void polyline(std::vector<double> x, std::vector<double> y,
                uint32_t fill = 0x000000, uint32_t stroke_width = 0,
                uint32_t stroke = 0x000000);
  void polygon(std::vector<std::array<double, 2>> points,
               uint32_t fill = 0x000000, uint32_t stroke_width = 0,
               uint32_t stroke = 0x000000);
  void polygon(std::vector<double> x, std::vector<double> y,
               uint32_t fill = 0x000000, uint32_t stroke_width = 0,
               uint32_t stroke = 0x000000);
  void triangle(double x1, double y1, double x2, double y2, double x3,
                double y3, uint32_t fill = 0x000000, uint32_t stroke_width = 0,
                uint32_t stroke = 0x000000);

  std::vector<std::pair<SVGTag, std::map<std::string, std::string>>> elements;
  uint64_t width_, height_;
};
}  // namespace image

#endif  // TRIGON_IMG_HPP_
