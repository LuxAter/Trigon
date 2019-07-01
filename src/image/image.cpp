#include "image.hpp"

#include <algorithm>
#include <cmath>
#include <map>
#include <string>
#include <vector>

#include "../color.hpp"
#include "../terminal.hpp"
#include "../util/util.hpp"
#include "jpeg.hpp"
#include "png.hpp"
#include "svg.hpp"

unsigned get_resolution(const unsigned& w, const std::string aspect) {
  unsigned ax = stol(aspect.substr(0, aspect.find(':')));
  unsigned ay = stol(aspect.substr(aspect.find(':') + 1));
  double ar = ay / static_cast<double>(ax);
  return static_cast<unsigned>(w * ar);
}

Image::Image(const std::string& file_path) {
  if (endswith(file_path, ".svg")) {
  } else if (endswith(file_path, ".png")) {
    auto res = read_png(file_path);
    width_ = res.first[0];
    height_ = res.first[1];
    pixels_ = res.second;
    type = RASTERIZED;
  } else if (endswith(file_path, ".jpeg")) {
  } else {
    error("Unrecognized input image type \"" + file_path + "\"");
  }
}
Image::Image(const unsigned& w, const unsigned& h, ImageType type)
    : type(type), width_(w), height_(h) {
  if (type == RASTERIZED) {
    pixels_ = std::vector<COLOR>(width_ * height_, 0);
  }
}

bool Image::save(const std::string& file_path) {
  if (endswith(file_path, ".svg") && type == SVG) {
    return write_svg(file_path, width_, height_, elements_);
  } else if (endswith(file_path, ".svg")) {
    error("Cannot save rasterized image as SVG");
  } else if (endswith(file_path, ".png")) {
    rasterize();
    return write_png(file_path, width_, height_, pixels_);
  } else if (endswith(file_path, ".jpeg")) {
    rasterize();
    // write_png(file_path, width_, height_, pixels_);
  } else {
    error("Unrecognize output image type \"" + file_path + "\"");
    return false;
  }
}

void Image::rasterize() {
  pixels_ = std::vector<COLOR>(width_ * height_, 0);
  for (auto& it : elements_) {
    if (it.first == "circle") {
      int radius = stol(it.second["r"]);
      unsigned cx = stol(it.second["cx"]);
      unsigned cy = stol(it.second["cy"]);
      uint32_t c = rgb(it.second["fill"]);
      int rsq = radius * radius;
      for (int y = -radius; y <= radius; ++y) {
        for (int x = -radius; x <= radius; ++x) {
          if (x * x + y * y < rsq) {
            pixels_[(cy + y) * width_ + (cx + x)] = c;
          }
        }
      }
    } else if (it.first == "ellipse") {
      int rx = stol(it.second["rx"]);
      int ry = stol(it.second["ry"]);
      unsigned cx = stol(it.second["cx"]);
      unsigned cy = stol(it.second["cy"]);
      uint32_t c = rgb(it.second["fill"]);
      double rxsq = rx * rx, rysq = ry * ry;
      for (int y = -ry; y <= ry; ++y) {
        for (int x = -rx; x <= rx; ++x) {
          if (((x * x) / rxsq) + ((y * y) / rysq) <= 1.0) {
            pixels_[(cy + y) * width_ + (cx + x)] = c;
          }
        }
      }
    } else if (it.first == "line") {
      line_rast(stol(it.second["x1"]), stol(it.second["y1"]),
                stol(it.second["x2"]), stol(it.second["y2"]),
                rgb(it.second["stroke"]));
    } else if (it.first == "polygon") {
      std::vector<double> xs, ys;
      std::string points = it.second["points"];
      std::size_t pos = 0;
      while (pos != std::string::npos) {
        std::size_t next = points.find(' ', pos);
        if (next == std::string::npos) {
          std::string token = points.substr(pos, points.size() - pos);
          std::size_t split = token.find(',');
          xs.push_back(stol(token.substr(0, split)));
          ys.push_back(stol(token.substr(split + 1, token.size() - split - 1)));
          pos = next;
        } else {
          std::string token = points.substr(pos, next - pos);
          std::size_t split = token.find(',');
          xs.push_back(stol(token.substr(0, split)));
          ys.push_back(stol(token.substr(split + 1, token.size() - split - 1)));
          pos = next + 1;
        }
      }
      polygon_rast(xs, ys, rgb(it.second["fill"]));
    } else if (it.first == "rect") {
      unsigned x = stol(it.second["x"]), y = stol(it.second["y"]),
               w = stol(it.second["width"]), h = stol(it.second["height"]);
      uint32_t c = rgb(it.second["fill"]);
      for (unsigned py = y; py < y + h; py++) {
        for (unsigned px = x; px < x + w; px++) {
          pixels_[py * width_ + px] = c;
        }
      }
    }
  }
}

void Image::fill(const COLOR& c) {
  if (type == RASTERIZED) {
    std::fill(pixels_.begin(), pixels_.end(), c);
  } else {
    elements_.clear();
    elements_.push_back({"rect",
                         {{"x", "0"},
                          {"y", "0"},
                          {"width", std::to_string(width_)},
                          {"height", std::to_string(height_)},
                          {"fill", fmt::get_hex(c)}}});
  }
}

void Image::circle(const double& cx, const double& cy, const double& r,
                   const COLOR& c) {
  if (type == RASTERIZED) {
  } else {
    elements_.push_back({"circle",
                         {{"cx", fmt::get_float(cx)},
                          {"cy", fmt::get_float(cy)},
                          {"r", fmt::get_float(r)},
                          {"fill", fmt::get_hex(c)},
                          {"stroke-width", "0"}}});
  }
}

void Image::ellipse(const double& cx, const double& cy, const double& rx,
                    const double& ry, const COLOR& c) {
  if (type == RASTERIZED) {
  } else {
    elements_.push_back({"ellipse",
                         {{"cx", fmt::get_float(cx)},
                          {"cy", fmt::get_float(cy)},
                          {"rx", fmt::get_float(rx)},
                          {"ry", fmt::get_float(ry)},
                          {"fill", fmt::get_hex(c)},
                          {"stroke-width", "0"}}});
  }
}

void Image::line(const double& x1, const double& y1, const double& x2,
                 const double& y2, const COLOR& c) {
  if (type == RASTERIZED) {
  } else {
    elements_.push_back({"line",
                         {
                             {"x1", fmt::get_float(x1)},
                             {"y1", fmt::get_float(y1)},
                             {"x2", fmt::get_float(x2)},
                             {"y2", fmt::get_float(y2)},
                             {"stroke", fmt::get_hex(c)},
                         }});
  }
}

void Image::polygon(const std::vector<std::array<double, 2>>& points,
                    const COLOR& c) {
  if (type == RASTERIZED) {
  } else {
    std::string points_str = "";
    for (auto& it : points) {
      points_str += fmt::get_float(it[0]) + ',' + fmt::get_float(it[1]) + ' ';
    }
    points_str.pop_back();
    elements_.push_back({"polygon",
                         {{"points", points_str},
                          {"fill", fmt::get_hex(c)},
                          {"stroke-width", "0"}}});
  }
}
void Image::polygon(const std::vector<double>& x, const std::vector<double>& y,
                    const COLOR& c) {
  if (type == RASTERIZED) {
  } else {
    std::string points_str = "";
    for (std::size_t i = 0; i < x.size() && i < y.size(); ++i) {
      points_str += fmt::get_float(x[0]) + ',' + fmt::get_float(y[1]) + ' ';
    }
    points_str.pop_back();
    elements_.push_back({"polygon",
                         {{"points", points_str},
                          {"fill", fmt::get_hex(c)},
                          {"stroke-width", "0"}}});
  }
}

void Image::rect(const double& x, const double& y, const double& w,
                 const double& h, const COLOR& c) {
  if (type == RASTERIZED) {
    for (unsigned py = std::floor(y); py < y + h; py++) {
      for (unsigned px = std::floor(x); px < x + w; px++) {
        pixels_[py * width_ + px] = c;
      }
    }
  } else {
    elements_.push_back({"rect",
                         {{"x", fmt::get_float(x)},
                          {"y", fmt::get_float(x)},
                          {"width", fmt::get_float(w)},
                          {"height", fmt::get_float(h)},
                          {"fill", fmt::get_hex(c)},
                          {"stroke-width", "0"}}});
  }
}

void Image::triangle(const double& x1, const double& y1, const double& x2,
                     const double& y2, const double& x3, const double& y3,
                     const COLOR& c) {
  polygon({{x1, y1}, {x2, y2}, {x3, y3}}, c);
}

void Image::line_rast(int x0, int y0, int x1, int y1, const COLOR& c) {
  if (std::abs(y1 - y0) < std::abs(x1 - x0)) {
    if (x0 > x1) {
      line_low(x1, y1, x0, y0, c);
    } else {
      line_low(x0, y0, x1, y1, c);
    }
  } else {
    if (y0 > y1) {
      line_high(x1, y1, x0, y0, c);
    } else {
      line_high(x0, y0, x1, y1, c);
    }
  }
}
void Image::line_low(int x0, int y0, int x1, int y1, const COLOR& c) {
  int dx = x1 - x0;
  int dy = y1 - y0;
  int yi = 1;
  if (dy < 0) {
    yi = -1;
    dy = -dy;
  }
  int d = 2 * dy - dx;
  int y = y0;
  for (int x = x0; x <= x1; ++x) {
    pixels_[y * width_ + x] = c;
    if (d > 0) {
      y += yi;
      d -= (2 * dx);
    }
    d += (2 * dy);
  }
}
void Image::line_high(int x0, int y0, int x1, int y1, const COLOR& c) {
  int dx = x1 - x0;
  int dy = y1 - y0;
  int xi = 1;
  if (dx < 0) {
    xi = -1;
    dx = -dx;
  }
  int d = 2 * dx - dy;
  int x = x0;
  for (int y = y0; y <= y1; ++y) {
    pixels_[y * width_ + x] = c;
    if (d > 0) {
      x += xi;
      d -= (2 * dy);
    }
    d += (2 * dx);
  }
}

void Image::polygon_rast(const std::vector<double>& x,
                         const std::vector<double>& y, const COLOR& c) {
  int min_y = y[0];
  int max_y = y[0];
  for (std::size_t i = 1; i < y.size(); ++i) {
    min_y = std::min(min_y, static_cast<int>(y[i]));
    max_y = std::max(max_y, static_cast<int>(y[i]));
  }
  for (int i = min_y; i <= max_y; ++i) {
    std::vector<int> inter;
    for (std::size_t j = 0; j < x.size() && j < y.size(); ++j) {
      std::size_t k = (j + 1) % std::min(x.size(), y.size());
      if (std::max(y[j], y[k]) > i && std::min(y[j], y[k]) <= i) {
        if (x[j] == x[k]) {
          inter.push_back(x[j]);
        } else {
          double m = static_cast<double>(y[k] - y[j]) / (x[k] - x[j]);
          inter.push_back(static_cast<int>((i - y[j] + (m * x[j])) / m));
        }
      }
    }
    std::sort(inter.begin(), inter.end());
    for (std::size_t a = 0; a < inter.size(); a += 2) {
      std::size_t b = (a + 1) % inter.size();
      if (inter[a] > inter[b]) {
        for (int xv = inter[b]; xv <= inter[a]; ++xv) {
          pixels_[i * width_ + xv] = c;
        }
      } else {
        for (int xv = inter[a]; xv <= inter[b]; ++xv) {
          pixels_[i * width_ + xv] = c;
        }
      }
    }
  }
}
