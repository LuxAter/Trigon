#ifndef TRIGON_IMAGE_PNG_HPP_
#define TRIGON_IMAGE_PNG_HPP_

#include <array>
#include <string>
#include <vector>

#include <png.h>

#include "../config.hpp"

std::pair<std::array<unsigned, 2>, std::vector<COLOR>> read_png(
    const std::string& file);
bool write_png(const std::string& file, const unsigned& w, const unsigned& h,
               const std::vector<COLOR>& pixels);

#endif  // TRIGON_IMAGE_PNG_HPP_
