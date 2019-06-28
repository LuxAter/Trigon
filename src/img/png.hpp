#ifndef TRIGON_IMG_PNG_HPP_
#define TRIGON_IMG_PNG_HPP_

#include <string>

#include "image.hpp"

namespace image {
bool write_png(const std::string& file_path, const Image& img);
}  // namespace image

#endif  // TRIGON_IMG_PNG_HPP_
