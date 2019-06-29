#include "image.hpp"

#include <cmath>
#include <map>
#include <string>
#include <vector>

#include "../terminal.hpp"
#include "../util/util.hpp"
#include "jpeg.hpp"
#include "png.hpp"
#include "svg.hpp"

Image::Image(const std::string& file_path) {
  if (endswith(file_path, ".svg")) {
  } else if (endswith(file_path, ".png")) {
    auto res = read_png(file_path);
    width_ = res.first[0];
    height_ = res.first[1];
    pixels_ = res.second;
  } else if (endswith(file_path, ".jpeg")) {
  } else {
    error("Unrecognized input image type \"" + file_path + "\"");
  }
}
Image::Image(const unsigned& w, const unsigned& h, ImageType type)
    : type(type), width_(w), height_(h) {
  if (type == RASTERIZED) {
    pixels_ = std::vector<COLOR>(width_ * height_, 0);
  }
}

bool Image::save(const std::string& file_path) {
  if (endswith(file_path, ".svg") && type == SVG) {
  } else if (endswith(file_path, ".svg")) {
    error("Cannot save rasterized image as SVG");
  } else if (endswith(file_path, ".png")) {
    rasterize();
    return write_png(file_path, width_, height_, pixels_);
  } else if (endswith(file_path, ".jpeg")) {
    rasterize();
    // write_png(file_path, width_, height_, pixels_);
  } else {
    error("Unrecognize output image type \"" + file_path + "\"");
    return false;
  }
}

void Image::rasterize() {}

void Image::fill(const COLOR& c) {
  if (type == RASTERIZED) {
    std::fill(pixels_.begin(), pixels_.end(), c);
  } else {
    elements_.clear();
    elements_.push_back(
        {"rect",
         {{"width", "100%"}, {"height", "100%"}, {"fill", fmt::get_hex(c)}}});
  }
}

void Image::rect(const double& x, const double& y, const double& w,
                 const double& h, const COLOR& c) {
  if (type == RASTERIZED) {
    for (unsigned py = std::floor(y); py <= y + h; py++) {
      for (unsigned px = std::floor(x); px <= x + w; px++) {
        pixels_[py * width_ + px] = c;
      }
    }
  }
}
