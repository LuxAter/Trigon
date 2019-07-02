#include "delaunay.hpp"

#include <random>

#include "../cxxopts.hpp"
#include "../image/image.hpp"
#include "../terminal.hpp"
#include "../util/util.hpp"

#define REAL double
#define VOID void

#include <triangle.h>

std::pair<std::vector<std::array<double, 2>>,
          std::vector<std::array<std::size_t, 3>>>
delaunay::execute_triangle(cxxopts::ParseResult args,
                           const std::vector<std::array<double, 2>>& points,
                           const std::vector<std::array<std::size_t, 2>>& edges,
                           const std::vector<std::array<double, 2>>& holes) {
  bool enforce_area = args.count("area");
  bool enforce_quality = args.count("quality");
  double area = -1.0;
  double quality = -1.0;
  if (enforce_area) {
    area = args["area"].as<double>();
  }
  if (enforce_quality) {
    quality = args["quality"].as<double>();
  }

  struct triangulateio src, out;
  src.numberofpoints = points.size();
  src.numberofsegments = edges.size();
  src.numberofholes = holes.size();
  src.numberofpointattributes = 0;
  src.numberofregions = 0;
  src.pointmarkerlist = NULL;
  src.segmentmarkerlist = NULL;

  out.pointlist = NULL;
  out.pointmarkerlist = NULL;
  out.trianglelist = NULL;
  out.neighborlist = NULL;
  out.segmentlist = NULL;
  out.edgelist = NULL;

  if (src.numberofpoints != 0) {
    src.pointlist = static_cast<double*>(
        std::malloc(src.numberofpoints * 2 * sizeof(double)));
    for (std::size_t i = 0; i < points.size(); ++i) {
      src.pointlist[2 * i] = points[i][0];
      src.pointlist[2 * i + 1] = points[i][1];
    }
  }
  if (src.numberofsegments != 0) {
    src.segmentlist =
        static_cast<int*>(std::malloc(src.numberofsegments * 2 * sizeof(int)));
    for (std::size_t i = 0; i < edges.size(); ++i) {
      src.segmentlist[2 * i] = edges[i][0];
      src.segmentlist[2 * i + 1] = edges[i][1];
    }
  }
  if (src.numberofholes != 0) {
    src.holelist = static_cast<double*>(
        std::malloc(src.numberofholes * 2 * sizeof(double)));
    for (std::size_t i = 0; i < holes.size(); ++i) {
      src.holelist[2 * i] = holes[i][0];
      src.holelist[2 * i + 1] = holes[i][1];
    }
  }

  std::string switches = "enzV";
  if (edges.size() != 0 || holes.size() != 0) {
    switches += "p";
  }
  if (enforce_area) {
    switches += "a" + fmt::get_float(area);
  }
  if (enforce_quality) {
    switches += "q" + fmt::get_float(quality);
  }
  info(switches);
  char* buff = static_cast<char*>(std::malloc(switches.size() + 1));
  std::copy(switches.begin(), switches.end(), buff);

  triangulate(buff, &src, &out, NULL);

  std::vector<std::array<double, 2>> out_points;
  std::vector<std::array<std::size_t, 3>> out_triangles;

  if (src.numberofpoints != 0) {
    free(src.pointlist);
  }
  if (src.numberofsegments != 0) {
    free(src.segmentlist);
  }
  if (src.numberofholes != 0) {
    free(src.holelist);
  }
  free(out.pointlist);
  free(out.pointmarkerlist);
  free(out.trianglelist);
  free(out.neighborlist);
  free(out.segmentlist);
  free(out.edgelist);

  return {out_points, out_triangles};
}

std::pair<std::vector<std::array<double, 2>>,
          std::vector<std::array<std::size_t, 3>>>
delaunay::generate_mesh(cxxopts::ParseResult args, const unsigned& w,
                        const unsigned& h) {
  std::vector<std::array<double, 2>> points, holes;
  std::vector<std::array<std::size_t, 2>> edges;
  if (args.count("pslg")) {
    info("NEED TO LOAD PSLG");
  } else {
    info("GENERATING PSLG");
    if (args.count("mesh")) {
      info("Generating mesh");
      double stddev = args["stddev"].as<double>();
      double spacing = args["spacing"].as<double>();
      std::random_device rd;
      std::mt19937 gen(rd());
      std::normal_distribution<double> dist(0.0, stddev);

      points.push_back({-5.0, -5.0});
      points.push_back({static_cast<double>(w) + 5.0, -5.0});
      points.push_back(
          {static_cast<double>(w) + 5.0, static_cast<double>(h) + 5.0});
      points.push_back({-5.0, static_cast<double>(h) + 5.0});
      for (double y = -5.0; y < h + 5.0; y += spacing) {
        for (double x = -5.0; x < w + 5.0; x += spacing) {
          points.push_back({x + dist(gen), y + dist(gen)});
        }
      }

    } else {
      points.push_back({-1.0, -1.0});
      points.push_back({static_cast<double>(w), -1.0});
      points.push_back({static_cast<double>(w), static_cast<double>(h)});
      points.push_back({-1.0, static_cast<double>(h)});
    }
  }
  return execute_triangle(args, points, edges, holes);
}

void delaunay::main(cxxopts::ParseResult args) {
  Image img(args["resolution"].as<unsigned>(),
            get_resolution(args["resolution"].as<unsigned>(),
                           args["aspect"].as<std::string>()));

  img.fill(0xffffff);

  auto returned = generate_mesh(args, img.width_, img.height_);
  auto points = returned.first;
  auto triangles = returned.second;

  img.save(args["output"].as<std::string>() + '.' +
           args["extension"].as<std::string>());
}
