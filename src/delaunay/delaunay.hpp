#ifndef TRIGON_DELAUNAY_HPP_
#define TRIGON_DELAUNAY_HPP_

#include "../opts.hpp"

namespace delaunay {
std::pair<std::vector<std::array<double, 2>>,
          std::vector<std::array<std::size_t, 3>>>
execute_triangle(opts::ParseResult args,
                 const std::vector<std::array<double, 2>>& points,
                 const std::vector<std::array<std::size_t, 2>>& edges,
                 const std::vector<std::array<double, 2>>& holes);
std::pair<std::vector<std::array<double, 2>>,
          std::vector<std::array<std::size_t, 3>>>
generate_mesh(opts::ParseResult args, const unsigned& w, const unsigned& h);
void main(opts::ParseResult args);
}  // namespace delaunay

#endif  //  TRIGON_DELAUNAY_HPP_
