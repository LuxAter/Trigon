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
delaunay::execute_triangle(const cxxopts::ParseResult* args,
                           const std::vector<std::array<double, 2>>& points,
                           const std::vector<std::array<std::size_t, 2>>& edges,
                           const std::vector<std::array<double, 2>>& holes) {
  bool enforce_area = args->count("area");
  bool enforce_quality = args->count("quality");
  double area = -1.0;
  double quality = -1.0;
  if (enforce_area) {
    area = (*args)["area"].as<double>();
  }
  if (enforce_quality) {
    quality = (*args)["quality"].as<double>();
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
    info("1");
    src.pointlist = static_cast<double*>(
        std::malloc(src.numberofpoints * 2 * sizeof(double)));
    for (std::size_t i = 0; i < points.size(); ++i) {
      src.pointlist[2 * i] = points[i][0];
      src.pointlist[2 * i + 1] = points[i][1];
    }
  }
  if (src.numberofsegments != 0) {
    info("2");
    src.segmentlist =
        static_cast<int*>(std::malloc(src.numberofsegments * 2 * sizeof(int)));
    for (std::size_t i = 0; i < edges.size(); ++i) {
      src.segmentlist[2 * i] = edges[i][0];
      src.segmentlist[2 * i + 1] = edges[i][1];
    }
  }
  if (src.numberofholes != 0) {
    info("3");
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
  char* buff = static_cast<char*>(std::malloc(switches.size() + 1));
  std::copy(switches.begin(), switches.end(), buff);

  success(switches);
  triangulate(buff, &src, &out, NULL);
  success(switches);

  std::vector<std::array<double, 2>> out_points;
  std::vector<std::array<std::size_t, 3>> out_triangles;

  for (std::size_t i = 0; i < out.numberofpoints; ++i) {
    out_points.push_back({out.pointlist[i * 2 + 0], out.pointlist[i * 2 + 1]});
  }
  for (std::size_t i = 0; i < out.numberoftriangles; ++i) {
    out_triangles.push_back(
        {static_cast<std::size_t>(out.trianglelist[i * 3 + 0]),
         static_cast<std::size_t>(out.trianglelist[i * 3 + 1]),
         static_cast<std::size_t>(out.trianglelist[i * 3 + 2])});
  }

  if (src.numberofpoints != 0) {
    info("1");
    free(src.pointlist);
    info("1");
  }
  if (src.numberofsegments != 0) {
    info("2");
    free(src.segmentlist);
  }
  if (src.numberofholes != 0) {
    info("3");
    free(src.holelist);
  }
  info("4");
  free(out.pointlist);
  info("5");
  free(out.pointmarkerlist);
  info("6");
  free(out.trianglelist);
  info("7");
  free(out.neighborlist);
  info("8");
  free(out.segmentlist);
  info("9");
  free(out.edgelist);

  return {out_points, out_triangles};
}

std::pair<std::vector<std::array<double, 2>>,
          std::vector<std::array<std::size_t, 3>>>
delaunay::generate_mesh(const cxxopts::ParseResult* args, const unsigned& w,
                        const unsigned& h) {
  std::vector<std::array<double, 2>> points, holes;
  std::vector<std::array<std::size_t, 2>> edges;
  if (args->count("pslg")) {
    info("NEED TO LOAD PSLG");
  } else {
    info("GENERATING PSLG");
    if (args->count("mesh")) {
      info("Generating mesh");
      double stddev = (*args)["stddev"].as<double>();
      double spacing = (*args)["spacing"].as<double>();
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
  auto res = execute_triangle(args, points, edges, holes);
  return res;
}

void delaunay::main(const cxxopts::ParseResult* args) {
  Image img((*args)["resolution"].as<unsigned>(),
            get_resolution((*args)["resolution"].as<unsigned>(),
                           (*args)["aspect"].as<std::string>()));

  img.fill(0xffffff);

  auto returned = generate_mesh(args, img.width_, img.height_);
  auto points = returned.first;
  auto triangles = returned.second;

  for (auto& it : triangles) {
    img.triangle(points[it[0]][0], points[it[0]][1], points[it[1]][0],
                 points[it[1]][1], points[it[2]][0], points[it[2]][1], 0x000000,
                 true);
  }

  img.save((*args)["output"].as<std::string>() + '.' +
           (*args)["extension"].as<std::string>());
}
