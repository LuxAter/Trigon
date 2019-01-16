#ifndef TRIGON_COLOR_H_
#define TRIGON_COLOR_H_

#include <cmath>
#include <cstdbool>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

struct Color {
  Color(uint32_t c)
      : r(static_cast<double>((c >> 16) & 0xFF) / 255.0),
        g(static_cast<double>((c >> 8) & 0xFF) / 255.0),
        b(static_cast<double>(c & 0xFF) / 255.0) {}
  Color(double r, double g, double b) : r(r), g(g), b(b) {}
  operator uint32_t() {
    return (static_cast<uint32_t>(r * 255.0) << 16) | (static_cast<uint32_t>(g * 255.0) << 8) |
           (static_cast<uint32_t>(b * 255.0));
  }
  double r, g, b;
};

void Print(const Color&);
void Print(const uint32_t&);

Color operator+(const Color& lhs, const Color& rhs) {
  return Color(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b);
}
Color operator-(const Color& lhs, const Color& rhs) {
  return Color(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b);
}
Color operator*(const Color& lhs, const double& rhs) {
  return Color(lhs.r * rhs, lhs.g * rhs, lhs.b * rhs);
}
std::ostream& operator<<(std::ostream& out, const Color& rgb) {
  out << rgb.r << ',' << rgb.g << ',' << rgb.b;
  return out;
}

inline uint32_t Gradient(const uint32_t& s, const uint32_t& e, double t) {
  Color cs(s), ce(e), cd = ce - cs;
  return (cd * t + cs);
}
inline uint32_t Gradient(const std::vector<uint32_t>& colors, double t) {
  double step = 1.0 / (colors.size() - 1);
  std::size_t index = t / step;
  t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
  return Gradient(colors[index], colors[(index + 1) > colors.size() ? index : index + 1], t);
}

inline void Print(const Color& c) { printf("%f,%f,%f\n", c.r, c.g, c.b); }
inline void Print(const uint32_t& c) {
  Color rgb(c);
  printf("\033[48;2;%d;%d;%dm  \033[0m", static_cast<int>(rgb.r * 255),
         static_cast<int>(rgb.g * 255), static_cast<int>(rgb.b * 255));
}

#endif  // TRIGON_COLOR_H_
