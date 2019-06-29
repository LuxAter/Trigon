#ifndef TRIGON_IMAGE_IMAGE_HPP_
#define TRIGON_IMAGE_IMAGE_HPP_

#include <map>
#include <string>
#include <vector>

#include "../config.hpp"

enum ImageType { SVG, RASTERIZED };
class Image {
 public:
  Image(const std::string& file_path);
  Image(const unsigned& w, const unsigned& h, ImageType type = SVG);

  bool save(const std::string& file_path);

  void rasterize();

  void fill(const COLOR& c);

  void rect(const double& x, const double& y, const double& w, const double& h,
            const COLOR& c);

  ImageType type;
  unsigned width_, height_;

  std::vector<COLOR> pixels_;
  std::vector<std::pair<std::string, std::map<std::string, std::string>>>
      elements_;
};

#endif  // TRIGON_IMAGE_IMAGE_HPP_
