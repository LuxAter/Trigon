#ifndef TRIGON_DELAUNAY_HPP_
#define TRIGON_DELAUNAY_HPP_

#include "../cxxopts.hpp"

namespace delaunay {
std::pair<std::vector<std::array<double, 2>>,
          std::vector<std::array<std::size_t, 3>>>
execute_triangle(const cxxopts::ParseResult* args,
                 const std::vector<std::array<double, 2>>& points,
                 const std::vector<std::array<std::size_t, 2>>& edges,
                 const std::vector<std::array<double, 2>>& holes);
std::pair<std::vector<std::array<double, 2>>,
          std::vector<std::array<std::size_t, 3>>>
generate_mesh(const cxxopts::ParseResult* args, const unsigned& w,
              const unsigned& h);
void main(const cxxopts::ParseResult* args);
}  // namespace delaunay

#endif  //  TRIGON_DELAUNAY_HPP_
