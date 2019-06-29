#ifndef TRIGON_TRIANGLE_HPP_
#define TRIGON_TRIANGLE_HPP_

#include "color.hpp"
#include "img/img.hpp"
#include "util/util.hpp"

#include <iostream>

void triangle(std::map<std::string, argparse::Variable> args) {
  std::vector<uint32_t> colors = gen_gradient(args["gradient"].get_string());
  std::cout << colors.size();
  image::Image img(args["resolution"].get_int(),
                   image::resolutions_[args["resolution"].get_int()]);
  img.fill(rgb("#263238"));
  for (std::size_t i = 0; i < img.width_; i += 5) {
    img.rect(i, 0, 6, img.height_, 0, 0, grad(i / (double)img.width_, colors));
  }
  if (args["extension"].get_string() == "svg") {
    image::write_svg(
        args["output"].get_string() + '.' + args["extension"].get_string(),
        img);
  }
}

#endif  // TRIGON_TRIANGLE_HPP_
