#include <algorithm>
#include <cmath>
#include <iostream>

#include "color.hpp"
// #include "cxxopts.hpp"
#include "terminal.hpp"

#include "image/image.hpp"
#include "util/util.hpp"

#include "delaunay/delaunay.hpp"

#include "opts.hpp"

int main(int argc, char* argv[]) {
  srand(time(NULL));
  opts::ArgumentParser parser;

  parser.add_option("mode", "Rendering method [delaunay,primative]",
                    opts::Value::STRING, true);
  parser.add_option("h,help", "Print this help message");

  parser.add_option("input", "Sorce image", opts::Value::STRING);
  parser.add_option("p,pallet", "Color pallet to generate image using",
                    opts::Value::STRING);
  parser.add_option("c,center", "Center for the radial gradiant",
                    opts::Value(opts::Value::STRING).implicit("0.5,0.5"));

  parser.add_option("pslg", "PSLG file to generate the contraints [.pslg,.svg]",
                    opts::Value::STRING);
  parser.add_option("a,area", "Impose maximum triangle area",
                    opts::Value::NUMBER);
  parser.add_option("q,quality", "Impose minimum interior angle",
                    opts::Value(opts::Value::STRING).implicit(20));
  parser.add_option("m,mesh", "should generate a structured mesh");
  parser.add_option("stddev", "Standard Deviation of the mesh",
                    opts::Value(10.0));
  parser.add_option("spacing", "Spacing between points of the mesh",
                    opts::Value(70.0));

  parser.add_option("o,output", "Output path", opts::Value("out"));
  parser.add_option("e,extension", "Output image type", opts::Value("svg"));
  parser.add_option("r,resolution", "Output image resolution",
                    opts::Value(1920));
  parser.add_option("aspect", "Aspect ratio of output image",
                    opts::Value("16:9"));
  parser.add_option("c,count", "Number of images to generate", opts::Value(1));

  parser.add_option("color", "When to use color in the terminal",
                    opts::Value("always"));

  auto args = parser.parse(argc, argv);

  if (args["help"].count) {
    // std::cout << options.help() << std::endl;
    return 0;
  }

  if (args["quality"].count) {
    if (args["quality"].as<double>() > 60) {
      error("'quality' > 60, triangulation will never complete");
      return 1;
    } else if (args["quality"].as<double>() > 28.6) {
      if (!verify_warning("'quality' > 28.8, triangulation may not complete")) {
        return 1;
      }
    }
  }

  if (args["mode"].count) {
    if (args["mode"].as<std::string>() == "delaunay") {
      delaunay::main(args);
    } else if (args["mode"].as<std::string>() == "primative") {
    } else {
      error("'mode' must be from [delaunay,primative]");
      return 1;
    }
  } else {
    error("'mode' is required");
    return 1;
  }
  for (auto& it : args) {
    std::cout << it.first << ":" << it.second.value << "\n";
  }
  return 0;
}
