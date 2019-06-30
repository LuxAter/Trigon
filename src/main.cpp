#include <algorithm>
#include <cmath>
#include <iostream>

#include "color.hpp"
#include "cxxopts.hpp"
#include "terminal.hpp"

#include "image/image.hpp"
#include "util/util.hpp"

int main(int argc, char* argv[]) {
  srand(time(NULL));
  try {
    std::vector<std::string> modes = {"delaunay", "primative"};
    cxxopts::Options options(argv[0], "Triangle based image generator");
    options.positional_help("[POSITIONAL]");

    options.add_options()("h,help", "Print this help message");
    options.add_options()("mode", "Rendering method [delaunay,primative]",
                          cxxopts::value<std::string>(), "MODE");

    options.add_options("Input")("input", "Source image",
                                 cxxopts::value<std::string>(), "IMAGE");

    options.add_options("Delaunay")(
        "mask", "Mask file to generate the contraints [.pslg,.svg]",
        cxxopts::value<std::string>(), "MASK")(
        "a,area", "Impose maximum triangle area", cxxopts::value<double>())(
        "q,quality", "Impose minimum interior angle",
        cxxopts::value<double>()->implicit_value("20"));

    options.add_options("Output")(
        "o,output", "Output path",
        cxxopts::value<std::string>()->default_value("out"))(
        "e,extension", "Output image type",
        cxxopts::value<std::string>()->default_value("svg"))(
        "r,resolution", "Output image resolution",
        cxxopts::value<unsigned>()->default_value("1920"))(
        "aspect", "Aspect ratio of output image",
        cxxopts::value<std::string>()->default_value("16:9"))(
        "c,count", "Number of images to generate",
        cxxopts::value<unsigned>()->default_value("1"));

    options.add_options("Terminal")(
        "color", "When to use color in the terminal",
        cxxopts::value<std::string>()->default_value("always"));

    options.parse_positional({"mode", "input"});
    auto result = options.parse(argc, argv);
    if (result.count("help")) {
      std::cout << options.help() << std::endl;
      exit(0);
    }
    if (result.count("mode")) {
      if (std::find(modes.begin(), modes.end(),
                    result["mode"].as<std::string>()) != modes.end()) {
      } else {
        throw cxxopts::OptionParseException(
            "‘mode’ must be from [delaunay,primative]");
      }
    } else {
      throw cxxopts::OptionParseException("‘mode’ is required");
    }

    if (result.count("quality")) {
      if (result["quality"].as<double>() > 60) {
        throw cxxopts::OptionParseException(
            "‘quality’ > 60, triangulation will never complete");
      }
      if (result["quality"].as<double>() > 28.6) {
        if (!verify_warning(
                "‘quality’ > 28.8, triangulation may not complete")) {
          return 1;
        }
      }
    }
  } catch (const cxxopts::OptionException& e) {
    error(e.what());
    return 1;
  }

  Image img(500, 500, SVG);
  img.fill(0x00ff00);
  img.rect(55, 23, 57, 90, 0xf0ff0f);
  img.circle(100, 84, 35, 0x0f00f0);
  img.ellipse(100, 200, 80, 20, 0xff0000);
  img.line(10, 10, 200, 15, 0x000000);
  img.triangle(100, 100, 150, 100, 200, 200, 0xFFFFFF);
  img.save("out.svg");
  img.save("out.png");

  return 0;
}
