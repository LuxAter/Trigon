#include "color.hpp"
#include "img/img.hpp"
#include "util/util.hpp"

#include <cmath>
#include <iostream>

int main(int argc, char* argv[]) {
  srand(time(NULL));
  argparse::add_argument('r', "resolution", 1920,
                         "resolution of the output image");
  argparse::add_argument('o', "output", "out",
                         "output path for the image or series");
  argparse::add_argument('e', "extension", "png", "file extension type");
  auto args = argparse::parse(argc, argv);
  image::Image img(args["resolution"].get_int(),
                   image::resolutions_[args["resolution"].get_int()]);
  double max_dist = std::sqrt(std::pow(img.width_ / 2.0, 2.0) +
                              std::pow(img.height_ / 2.0, 2.0));
  std::vector<uint32_t> colors = {rgb("#ff0000"), rgb("#00ff00"),
                                  rgb("#0000ff")};
  // std::vector<uint32_t> colors = {0xff0000, 0x00ff00, 0x0000ff};
  for (uint32_t y = 0; y < img.height_; ++y) {
    for (uint32_t x = 0; x < img.width_; ++x) {
      // double dist = x / (double)img.width_;
      double dist = std::sqrt(std::pow(x - img.width_ / 2.0, 2.0) +
                              std::pow(y - img.height_ / 2.0, 2.0)) /
                    max_dist;
      img(x, y) = grad(dist, colors);
    }
    // printf("%x\n", img(0, y));
  }
  if (!image::write_png(
          fmt::format("{0}.{1}", {args["output"].get_string(),
                                  args["extension"].get_string()}),
          img)) {
    std::cout << "FAILED\n";
  }
  return 0;
}
