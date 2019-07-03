#ifndef TRIGON_IMAGE_IMAGE_HPP_
#define TRIGON_IMAGE_IMAGE_HPP_

#include <map>
#include <string>
#include <vector>

#include "../config.hpp"

enum ImageType { SVG, RASTERIZED };

unsigned get_resolution(const unsigned& w, const std::string aspect);

class Image {
 public:
  Image(const std::string& file_path);
  Image(const unsigned& w, const unsigned& h, ImageType type = SVG);

  bool save(const std::string& file_path);

  void rasterize();

  void fill(const COLOR& c);

  void circle(const double& cx, const double& cy, const double& r,
              const COLOR& c);
  void ellipse(const double& cx, const double& cy, const double& rx,
               const double& ry, const COLOR& c);
  void line(const double& x1, const double& y1, const double& x2,
            const double& y2, const COLOR& c);
  void polygon(const std::vector<std::array<double, 2>>& points, const COLOR& c,
               bool outline = false);
  void polygon(const std::vector<double>& x, const std::vector<double>& y,
               const COLOR& c);
  void rect(const double& x, const double& y, const double& w, const double& h,
            const COLOR& c);

  void triangle(const double& x1, const double& y1, const double& x2,
                const double& y2, const double& x3, const double& y3,
                const COLOR& c, bool outline = false);

  ImageType type;
  unsigned width_, height_;

  std::vector<COLOR> pixels_;
  std::vector<std::pair<std::string, std::map<std::string, std::string>>>
      elements_;

 private:
  void line_rast(int x0, int y0, int x1, int y1, const COLOR& c);
  void line_high(int x0, int y0, int x1, int y1, const COLOR& c);
  void line_low(int x0, int y0, int x1, int y1, const COLOR& c);

  void polygon_rast(const std::vector<double>& x, const std::vector<double>& y,
                    const COLOR& c);
};

#endif  // TRIGON_IMAGE_IMAGE_HPP_
