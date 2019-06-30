#ifndef TRIGON_IMAGE_SVG_HPP_
#define TRIGON_IMAGE_SVG_HPP_

#include <array>
#include <map>
#include <string>
#include <vector>

#include "../config.hpp"

std::pair<std::array<unsigned, 2>, std::vector<COLOR>> read_svg(
    const std::string& file);
bool write_svg(
    const std::string& file, const unsigned& w, const unsigned& h,
    std::vector<std::pair<std::string, std::map<std::string, std::string>>>
        elements);

#endif  // TRIGON_IMAGE_SVG_HPP_
