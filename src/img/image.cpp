#include "image.hpp"

#include <cstdlib>

#include <algorithm>

image::Image::Image(const uint64_t& w, const uint64_t& h)
    : width_(w),
      height_(h),
      src(static_cast<uint32_t*>(std::malloc(sizeof(uint32_t) * w * h))) {
  std::fill(src, src + (width_ * height_), 0x000000);
}

image::Image::~Image() { free(src); }

void image::Image::fill(const uint32_t& c) {
  std::fill(src, src + (width_ * height_), c);
}

uint32_t& image::Image::operator()(const uint64_t& x, const uint64_t& y) {
  return src[y * width_ + x];
}
uint32_t image::Image::operator()(const uint64_t& x, const uint64_t& y) const {
  return src[y * width_ + x];
}
