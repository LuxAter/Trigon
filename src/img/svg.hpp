#ifndef TRIGON_IMG_SVG_HPP_
#define TRIGON_IMG_SVG_HPP_

#include <string>
#include "image.hpp"

namespace image {
std::string get_tag(const SVGTag& tag);
bool write_svg(const std::string& file_path, const Image& img);
}  // namespace image

#endif  // TRIGON_IMG_SVG_HPP_
