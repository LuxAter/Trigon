#include "color.hpp"

#include <cmath>
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "terminal.hpp"

std::map<std::string, std::vector<uint32_t>> material_colors_ = {
    {"red",
     {0xffebee, 0xffcdd2, 0xef9a9a, 0xe57373, 0xef5350, 0xf44336, 0xe53935,
      0xd32f2f, 0xc62828, 0xb71c1c}},
    {"pink",
     {0xfce4ec, 0xf8bbd0, 0xf48fb1, 0xf06292, 0xec407a, 0xe91e63, 0xd81b60,
      0xc2185b, 0xad1457, 0x880e4f}},
    {"purple",
     {0xf3e5f5, 0xe1bee7, 0xce93d8, 0xba68c8, 0xab47bc, 0x9c26b0, 0x8e24aa,
      0x7b1fa2, 0x6a1b9a, 0x4a148c}},
    {"deep-purple",
     {0xede7f6, 0xd1c4e9, 0xb39ddb, 0x9575cd, 0x7e57c2, 0x673ab7, 0x5e35b1,
      0x512da8, 0x4527a0, 0x311b92}},
    {"indigo",
     {0xe8eaf6, 0xc5cae9, 0x9fa8da, 0x7986cb, 0x5c6bc0, 0x3f51b5, 0x3949ab,
      0x303f9f, 0x283593, 0x1a237e}},
    {"blue",
     {0xe2f2fd, 0xbbdefb, 0x90caf9, 0x64b5f6, 0x43a5f5, 0x2196f3, 0x1e88e5,
      0x1976d2, 0x1565c0, 0x0d47a1}},
    {"light-blue",
     {0xe1f5fe, 0xb3e5fc, 0x81d4fa, 0x4fc3f7, 0x29b6f6, 0x03a9f4, 0x039be5,
      0x0288d1, 0x0277bd, 0x01579b}},
    {"cyan",
     {0xe0f7fa, 0xb2ebf2, 0x80deea, 0x4dd0e1, 0x26c6da, 0x00bcd4, 0x00acc1,
      0x0097a7, 0x00838f, 0x006064}},
    {"teal",
     {0xe0f2f1, 0xb2dfdb, 0x80cbc4, 0x4db6ac, 0x26a69a, 0x009688, 0x00987b,
      0x00796b, 0x00695c, 0x004d40}},
    {"green",
     {0xe8f5e9, 0xc8e6c9, 0xa5d6a7, 0x81c784, 0x66bb6a, 0x4caf50, 0x43a047,
      0x388e3c, 0x2e7d32, 0x1b5e20}},
    {"light-green",
     {0xf1f8e9, 0xdcedc8, 0xc5e1a5, 0xaed581, 0x9ccc65, 0x8bc34a, 0x7cb342,
      0x689f38, 0x558b2f, 0x33691e}},
    {"lime",
     {0xf9fbe7, 0xf0f4c3, 0xe6ee9c, 0xdce775, 0xd4e157, 0xcddc39, 0xc0ca33,
      0xafb42b, 0x9e9d24, 0x827717}},
    {"yellow",
     {0xfffde7, 0xfff9c4, 0xfff59d, 0xfff176, 0xffee58, 0xffeb3b, 0xfdd835,
      0xfbc02d, 0xf9a825, 0xf57f17}},
    {"amber",
     {0xfff8e1, 0xffecb3, 0xffe082, 0xffd54f, 0xffca28, 0xffc107, 0xffb300,
      0xffa000, 0xff8f00, 0xff6f00}},
    {"orange",
     {0xfff3e0, 0xffe0b2, 0xffcc80, 0xffb74d, 0xffa726, 0xff9800, 0xfb8c00,
      0xf57c00, 0xef6c00, 0xe65100}},
    {"deep-orange",
     {0xfbe9e7, 0xffccbc, 0xffab91, 0xff8a65, 0xff7043, 0xff5722, 0xf4511e,
      0xe64a19, 0xd84315, 0xbf360c}},
    {"brown",
     {0xefebe9, 0xd7ccc8, 0xbcaaa4, 0xa1887f, 0x8d6e63, 0x795548, 0x6d4c41,
      0x5d4037, 0x4e342e, 0x3e2723}},
    {"grey",
     {0xfafafa, 0xf5f5f5, 0xeeeeee, 0xe0e0e0, 0xbdbdbd, 0x9e9e9e, 0x757575,
      0x616161, 0x424242, 0x212121}},
    {"blue-grey",
     {0xeceff1, 0xcfd8dc, 0xb0bec5, 0x90a4ae, 0x78909c, 0x607d8b, 0x546e7a,
      0x455a64, 0x37474f, 0x263238}}};

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

std::vector<uint32_t> get_pallet(const std::string& palstr) {
  std::vector<uint32_t> pallet;
  std::size_t pos = 0;
  while (palstr.find(':', pos) != std::string::npos) {
    std::string current = palstr.substr(pos, palstr.find(':', pos) - pos);
    if (current[0] == '#') {
      pallet.push_back(rgb(current));
    } else if (current[current.find_last_of('-') + 1] <= 57) {
      std::string color = current.substr(0, current.find_last_of('-'));
      std::size_t id = stoul(current.substr(current.find_last_of('-') + 1));
      std::map<std::string, std::vector<uint32_t>>::iterator it;
      if ((it = material_colors_.find(color)) != material_colors_.end()) {
        pallet.push_back(it->second[id]);
      }
    } else if (current.back() != 's') {
      std::string color = current;
      std::size_t id = 5;
      std::map<std::string, std::vector<uint32_t>>::iterator it;
      if ((it = material_colors_.find(color)) != material_colors_.end()) {
        pallet.push_back(it->second[id]);
      }
    } else {
      bool inv = current[0] == 'n';
      std::string color = current.substr(inv, current.size() - 1 - inv);
      info(color);
      std::map<std::string, std::vector<uint32_t>>::iterator it;
      if ((it = material_colors_.find(color)) != material_colors_.end()) {
        if (!inv) {
          for (auto& cit : it->second) {
            pallet.push_back(cit);
          }
        } else {
          for (int i = it->second.size() - 1; i >= 0; --i) {
            pallet.push_back(it->second[i]);
          }
        }
      }
    }
    pos += current.size() + 1;
  }
  std::string current = palstr.substr(pos);
  if (current[0] == '#') {
    pallet.push_back(rgb(current));
  } else if (current[current.find_last_of('-') + 1] <= 57) {
    std::string color = current.substr(0, current.find_last_of('-'));
    std::size_t id = stoul(current.substr(current.find_last_of('-') + 1));
    std::map<std::string, std::vector<uint32_t>>::iterator it;
    if ((it = material_colors_.find(color)) != material_colors_.end()) {
      pallet.push_back(it->second[id]);
    }
  } else if (current.back() != 's') {
    std::string color = current;
    std::size_t id = 5;
    std::map<std::string, std::vector<uint32_t>>::iterator it;
    if ((it = material_colors_.find(color)) != material_colors_.end()) {
      pallet.push_back(it->second[id]);
    }
  } else {
    bool inv = current[0] == 'n';
    std::string color = current.substr(inv, current.size() - 1 - inv);
    std::map<std::string, std::vector<uint32_t>>::iterator it;
    if ((it = material_colors_.find(color)) != material_colors_.end()) {
      if (!inv) {
        for (auto& cit : it->second) {
          pallet.push_back(cit);
        }
      } else {
        for (int i = it->second.size() - 1; i >= 0; --i) {
          pallet.push_back(it->second[i]);
        }
      }
    }
  }
  return pallet;
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
  char rs[3] = "00", gs[3] = "00", bs[3] = "00";
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
    default:
      return grad_rgb(t, a, b);
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
    default:
      return grad_rgb(t, colors);
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
