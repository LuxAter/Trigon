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

  for (auto& it : args) {
    std::cout << it.first << ":" << it.second.value << "\n";
  }
  // try {
  //   std::vector<std::string> modes = {"delaunay", "primative"};
  //   cxxopts::Options options(argv[0], "Triangle based image generator");
  //   options.positional_help("[POSITIONAL]");
  //
  //   options.add_options()("h,help", "Print this help message");
  //   options.add_options()("mode", "Rendering method [delaunay,primative]",
  //                         cxxopts::value<std::string>(), "MODE");
  //
  //   options.add_options("Input")("input", "Source image",
  //                                cxxopts::value<std::string>(), "IMAGE");
  //
  //   options.add_options("Delaunay")(
  //       "pslg", "PSLG file to generate the contraints [.pslg,.svg]",
  //       cxxopts::value<std::string>(), "PSLG")(
  //       "a,area", "Impose maximum triangle area", cxxopts::value<double>())(
  //       "q,quality", "Impose minimum interior angle",
  //       cxxopts::value<double>())( "m,mesh", "Should generate a structured
  //       mesh")( "stddev", "Standard Deviation of the mesh variance",
  //       cxxopts::value<double>()->default_value("10.0"))(
  //       "spacing", "Spacing between points of the mesh",
  //       cxxopts::value<double>()->default_value("70"));
  //
  //   options.add_options("Output")(
  //       "o,output", "Output path",
  //       cxxopts::value<std::string>()->default_value("out"))(
  //       "e,extension", "Output image type",
  //       cxxopts::value<std::string>()->default_value("svg"))(
  //       "r,resolution", "Output image resolution",
  //       cxxopts::value<unsigned>()->default_value("1920"))(
  //       "aspect", "Aspect ratio of output image",
  //       cxxopts::value<std::string>()->default_value("16:9"))(
  //       "c,count", "Number of images to generate",
  //       cxxopts::value<unsigned>()->default_value("1"));
  //
  //   options.add_options("Terminal")(
  //       "color", "When to use color in the terminal",
  //       cxxopts::value<std::string>()->default_value("always"));
  //
  //   options.parse_positional({"mode", "input"});
  //
  //   auto result = options.parse(argc, argv);
  //
  //   // HELP
  //   if (result.count("help")) {
  //     std::cout << options.help() << std::endl;
  //     exit(0);
  //   }
  //
  //   // VALIDATION
  //   if (result.count("quality")) {
  //     // if (result["quality"].as<double>() > 60) {
  //     //   throw cxxopts::OptionParseException(
  //     //       "‘quality’ > 60, triangulation will never complete");
  //     // }
  //     if (result["quality"].as<double>() > 28.6) {
  //       if (!verify_warning(
  //               "‘quality’ > 28.8, triangulation may not complete")) {
  //         return 1;
  //       }
  //     }
  //   }
  //
  //   if (result.count("mode")) {
  //     if (result["mode"].as<std::string>() == "delaunay") {
  //       delaunay::main(&result);
  //       std::cout << "BACK!\n";
  //       return 0;
  //     } else if (result["mode"].as<std::string>() == "primative") {
  //       return 0;
  //     } else {
  //       throw cxxopts::OptionParseException(
  //           "‘mode’ must be from [delaunay,primative]");
  //     }
  //   } else {
  //     throw cxxopts::OptionParseException("‘mode’ is required");
  //   }
  //
  // } catch (const cxxopts::OptionException& e) {
  //   error(e.what());
  //   return 1;
  // }
  return 0;
}
