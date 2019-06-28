#ifndef TRIGON_IMG_HPP_
#define TRIGON_IMG_HPP_

#include <cstdint>
#include <map>

namespace image {
static std::map<uint32_t, uint32_t> resolutions_{{1920, 1080}};

class Image {
 public:
  Image(const uint64_t& w, const uint64_t& h);
  ~Image();

  void fill(const uint32_t& c);

  uint32_t& operator()(const uint64_t& x, const uint64_t& y);
  uint32_t operator()(const uint64_t& x, const uint64_t& y) const;

  uint64_t width_, height_;
  uint32_t* src;
};
}  // namespace image

#endif  // TRIGON_IMG_HPP_
