#include "util/util.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  srand(time(NULL));
  argparse::add_argument('r', "resolution", 1920,
                         "resolution of the output image");
  argparse::add_argument('o', "output", "out",
                         "output path for the image or series");
  argparse::add_argument('e', "extension", "png", "file extension type");
  auto args = argparse::parse(argc, argv);
  std::cout << args["resolution"].get_int();
  return 0;
}
