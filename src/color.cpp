#include "color.hpp"

#include <cmath>
#include <cstring>
#include <string>

double angle_diff_clockwise(double start, double end) {
  if (start <= end) {
    return end - start;
  } else {
    return 360.0 + end - start;
  }
}
double angle_diff_counter_clockwise(double start, double end) {
  if (end <= start) {
    return end - start;
  } else {
    return -360.0 - start + end;
  }
}
double angle_diff_min(double start, double end) {
  double cw = angle_diff_clockwise(start, end);
  double ccw = angle_diff_counter_clockwise(start, end);
  if (fabs(cw) > fabs(ccw)) {
    return ccw;
  } else {
    return cw;
  }
}
double angle_diff_max(double start, double end) {
  double cw = angle_diff_clockwise(start, end);
  double ccw = angle_diff_counter_clockwise(start, end);
  if (fabs(cw) > fabs(ccw)) {
    return cw;
  } else {
    return ccw;
  }
}

uint32_t rgb(const double& r, const double& g, const double& b) {
  return (static_cast<uint32_t>(r * 255.0) << 16) |
         (static_cast<uint32_t>(g * 255.0) << 8) |
         (static_cast<uint32_t>(b * 255.0) << 0);
}
uint32_t rgb(const uint8_t& r, const uint8_t& g, const uint8_t& b) {
  return (r << 16) | (g << 8) | (b << 0);
}
uint32_t rgb(const std::string& hex) {
  bool pound = (hex[0] == '#');
  char rs[3], gs[3], bs[3];
  strncpy(rs, hex.c_str() + 0 + pound, 2);
  strncpy(gs, hex.c_str() + 2 + pound, 2);
  strncpy(bs, hex.c_str() + 4 + pound, 2);
  return (strtoul(rs, NULL, 16) << 16) | (strtoul(gs, NULL, 16) << 8) |
         (strtoul(bs, NULL, 16) << 0);
}
uint32_t rgb(const std::vector<double>& c) { return rgb(c[0], c[1], c[2]); }
uint32_t rgb(const std::vector<uint8_t>& c) { return rgb(c[0], c[1], c[2]); }
std::vector<double> get_rgb(const uint32_t& c) {
  return std::vector<double>{((c >> 16) & 0xFF) / 255.0,
                             ((c >> 8) & 0xFF) / 255.0,
                             ((c >> 0) & 0xFF) / 255.0};
}
std::vector<uint8_t> get_rgbi(const uint32_t& c) {
  return std::vector<uint8_t>{static_cast<uint8_t>((c >> 16) & 0xFF),
                              static_cast<uint8_t>((c >> 8) & 0xFF),
                              static_cast<uint8_t>((c >> 0) & 0xFF)};
}

uint32_t cmyk(const double& c, const double& m, const double& y,
              const double& k) {
  double r = (1.0 - c) * (1.0 - k);
  double g = (1.0 - m) * (1.0 - k);
  double b = (1.0 - y) * (1.0 - k);
  return (static_cast<uint32_t>(r * 255.0) << 16) |
         (static_cast<uint32_t>(g * 255.0) << 8) |
         (static_cast<uint32_t>(b * 255.0) << 0);
}
uint32_t cmyk(const uint8_t& c, const uint8_t& m, const uint8_t& y,
              const uint8_t& k) {
  double r = (1.0 - (c / 255.0)) * (1.0 - (k / 255.0));
  double g = (1.0 - (c / 255.0)) * (1.0 - (k / 255.0));
  double b = (1.0 - (c / 255.0)) * (1.0 - (k / 255.0));
  return (static_cast<uint32_t>(r * 255.0) << 16) |
         (static_cast<uint32_t>(g * 255.0) << 8) |
         (static_cast<uint32_t>(b * 255.0) << 0);
}
uint32_t cmyk(const std::vector<double>& c) {
  return cmyk(c[0], c[1], c[2], c[3]);
}
uint32_t cmyk(const std::vector<uint8_t>& c) {
  return cmyk(c[0], c[1], c[2], c[3]);
}
std::vector<double> get_cmyk(const uint32_t& col) {
  std::vector<double> crgb = get_rgb(col);
  double r = crgb[0], g = crgb[1], b = crgb[2];
  double k = 1.0 - std::fmax(r, std::fmax(g, b));
  double c = (1.0 - r - k) / (1.0 - k);
  double m = (1.0 - g - k) / (1.0 - k);
  double y = (1.0 - b - k) / (1.0 - k);
  return {c, m, y, k};
}
std::vector<uint8_t> get_cmyki(const uint32_t& col) {
  std::vector<double> crgb = get_rgb(col);
  double r = crgb[0], g = crgb[1], b = crgb[2];
  double k = 1.0 - std::fmax(r, std::fmax(g, b));
  double c = (1.0 - r - k) / (1.0 - k);
  double m = (1.0 - g - k) / (1.0 - k);
  double y = (1.0 - b - k) / (1.0 - k);
  return {static_cast<uint8_t>(c * 255.0), static_cast<uint8_t>(m * 255.0),
          static_cast<uint8_t>(y * 255.0), static_cast<uint8_t>(k * 255.0)};
}

uint32_t hsv(const double& h, const double& s, const double& v) {
  double ih = std::fmod(h, 360.0);
  if (ih < 0) ih += 360.0;
  double c = v * s;
  double x = c * (1.0 - std::fabs(std::fmod(ih / 60.0, 2.0) - 1.0));
  double m = v - c;
  double r = 0.0, g = 0.0, b = 0.0;
  if (ih < 60.0) {
    r = c + m;
    g = x + m;
    b = m;
  } else if (ih < 120.0) {
    r = x + m;
    g = c + m;
    b = m;
  } else if (ih < 180.0) {
    r = m;
    g = c + m;
    b = x + m;
  } else if (ih < 240.0) {
    r = m;
    g = x + m;
    b = c + m;
  } else if (ih < 300.0) {
    r = x + m;
    g = m;
    b = c + m;
  } else {
    r = c + m;
    g = m;
    b = x + m;
  }
  return (static_cast<uint32_t>(r * 255.0) << 16) |
         (static_cast<uint32_t>(g * 255.0) << 8) |
         (static_cast<uint32_t>(b * 255.0) << 0);
}
uint32_t hsv(const std::vector<double>& c) { return hsv(c[0], c[1], c[2]); }
std::vector<double> get_hsv(const uint32_t& c) {
  std::vector<double> crgb = get_rgb(c);
  double r = crgb[0], g = crgb[1], b = crgb[2];
  double c_max = std::fmax(r, std::fmax(g, b));
  double c_min = std::fmin(r, std::fmin(g, b));
  double delta = c_max - c_min;
  double h = 0, s = 0, v = 0;
  if (delta == 0) {
    h = 0;
  } else if (c_max == r) {
    h = 60.0 * std::fmod((g - b) / delta, 6.0);
  } else if (c_max == g) {
    h = 60.0 * ((b - r) / delta + 2.0);
  } else if (c_max == b) {
    h = 60.0 * ((r - g) / delta + 4.0);
  }
  if (h < 0) {
    h += 360.0;
  }
  if (c_max == 0) {
    s = 0;
  } else {
    s = delta / c_max;
  }
  v = c_max;
  return {h, s, v};
}

uint32_t hsl(const double& h, const double& s, const double& l) {
  double ih = std::fmod(h, 360.0);
  if (ih < 0) ih += 360.0;
  double c = (1.0 - std::fabs(2.0 * l - 1.0)) * s;
  double x = c * (1.0 - std::fabs(std::fmod(ih / 60.0, 2.0) - 1.0));
  double m = l - c / 2.0;
  double r = 0.0, g = 0.0, b = 0.0;
  if (ih < 60.0) {
    r = c + m;
    g = x = m;
    b = m;
  } else if (ih < 120.0) {
    r = x + m;
    g = c + m;
    b = m;
  } else if (ih < 180.0) {
    r = m;
    g = c + m;
    b = x + m;
  } else if (ih < 240.0) {
    r = m;
    g = x + m;
    b = c + m;
  } else if (ih < 300.0) {
    r = x + m;
    g = m;
    b = c + m;
  } else {
    r = c + m;
    g = m;
    b = x + m;
  }
  return (static_cast<uint32_t>(r * 255.0) << 16) |
         (static_cast<uint32_t>(g * 255.0) << 8) |
         (static_cast<uint32_t>(b * 255.0) << 0);
}
uint32_t hsl(const std::vector<double>& c) { return hsl(c[0], c[1], c[2]); }
std::vector<double> get_hsl(const uint32_t& c) {
  std::vector<double> crgb = get_rgb(c);
  double r = crgb[0], g = crgb[1], b = crgb[2];
  double c_max = std::fmax(r, std::fmax(g, b));
  double c_min = std::fmin(r, std::fmin(g, b));
  double delta = c_max - c_min;
  double h = 0, s = 0, l = 0;
  if (delta == 0) {
    h = 0;
  } else if (c_max == r) {
    h = 60.0 * std::fmod((g - b) / delta, 6.0);
  } else if (c_max == g) {
    h = 60.0 * ((b - r) / delta + 2);
  } else if (c_max == b) {
    h = 60.0 * ((r - g) / delta + 4);
  }
  if (h < 0) {
    h += 360.0;
  }
  l = (c_max + c_max) / 2.0;
  if (c_max == 0) {
    s = 0;
  } else {
    s = delta / (1.0 - std::fabs(2.0 * l - 1.0));
  }
  return {h, s, l};
}

uint32_t grad(const double& t, const uint32_t& a, const uint32_t& b,
              ColorSpace method) {
  switch (method) {
    case RGB:
      return grad_rgb(t, a, b);
    case CMYK:
      return grad_cmyk(t, a, b);
    case HSV:
      return grad_hsv(t, a, b);
    case HSL:
      return grad_hsl(t, a, b);
  }
}
uint32_t grad(const double& t, std::vector<uint32_t> colors,
              ColorSpace method) {
  switch (method) {
    case RGB:
      return grad_rgb(t, colors);
    case CMYK:
      return grad_cmyk(t, colors);
    case HSV:
      return grad_hsv(t, colors);
    case HSL:
      return grad_hsl(t, colors);
  }
}

uint32_t grad_rgb(const double& t, const uint32_t& a, const uint32_t& b) {
  std::vector<double> start = get_rgb(a), end = get_rgb(b);
  double dr = end[0] - start[0];
  double dg = end[1] - start[1];
  double db = end[2] - start[2];
  return rgb((dr * t) + start[0], (dg * t) + start[1], (db * t) + start[2]);
}
uint32_t grad_rgb(const double& t, std::vector<uint32_t> colors) {
  double it = t * (colors.size() - 1);
  std::size_t index = std::floor(it);
  if (index == colors.size() - 1) {
    return colors.back();
  } else {
    return grad_rgb(it - index, colors[index], colors[index + 1]);
  }
}

uint32_t grad_cmyk(const double& t, const uint32_t& a, const uint32_t& b) {
  std::vector<double> start = get_cmyk(a), end = get_cmyk(b);
  double dc = end[0] - start[0];
  double dm = end[1] - start[1];
  double dy = end[2] - start[2];
  double dk = end[3] - start[3];
  return cmyk((dc * t) + start[0], (dm * t) + start[1], (dy * t) + start[2],
              (dk * t) + start[3]);
}
uint32_t grad_cmyk(const double& t, std::vector<uint32_t> colors) {
  double it = t * (colors.size() - 1);
  std::size_t index = std::floor(it);
  if (index == colors.size() - 1) {
    return colors.back();
  } else {
    return grad_cmyk(it - index, colors[index], colors[index + 1]);
  }
}

uint32_t grad_hsv(const double& t, const uint32_t& a, const uint32_t& b) {
  std::vector<double> start = get_hsv(a), end = get_hsv(b);
  double dh = angle_diff_min(start[0], end[0]);
  double ds = end[1] - start[1];
  double dv = end[2] - start[2];
  return hsv((dh * t) + start[0], (ds * t) + start[1], (dv * t) + start[2]);
}
uint32_t grad_hsv(const double& t, std::vector<uint32_t> colors) {
  double it = t * (colors.size() - 1);
  std::size_t index = std::floor(it);
  if (index >= colors.size() - 1) {
    return colors.back();
  } else {
    return grad_hsv(it - index, colors[index], colors[index + 1]);
  }
}

uint32_t grad_hsl(const double& t, const uint32_t& a, const uint32_t& b) {
  std::vector<double> start = get_hsl(a), end = get_hsl(b);
  double dh = angle_diff_min(start[0], end[0]);
  double ds = end[1] - start[1];
  double dl = end[2] - start[2];
  return hsl((dh * t) + start[0], (ds * t) + start[1], (dl * t) + start[2]);
}
uint32_t grad_hsl(const double& t, std::vector<uint32_t> colors) {
  double it = t * (colors.size() - 1);
  std::size_t index = std::floor(it);
  if (index == colors.size() - 1) {
    return colors.back();
  } else {
    return grad_hsl(it - index, colors[index], colors[index + 1]);
  }
}

std::vector<uint32_t> gen_gradient(const std::string& colors) {
  if (colors.size() == 0) {
    return std::vector<uint32_t>{0x000000, 0xffffff};
  }
  std::vector<uint32_t> grad;
  std::string s = colors;
  std::size_t pos = 0;
  std::string token;
  while ((pos = s.find(':')) != std::string::npos) {
    token = s.substr(0, pos);
    grad.push_back(rgb(token));
    s.erase(0, pos + 1);
  }
  grad.push_back(rgb(s));
  return grad;
}
