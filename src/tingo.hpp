#ifndef TINGO_H_
#define TINGO_H_

#ifdef __cplusplus
#include <cmath>
#include <cstdbool>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#else
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#ifdef __cplusplus
extern "C" {
namespace tingo {
#endif

  typedef enum { CS_RGB, CS_CMYK, CS_HSV, CS_HSL, CS_LAB } ColorSpace;
  typedef struct Color {
    ColorSpace color_space;
    double r, g, b, a;
    double h, s, l, v;
    double c, m, y, k;
  } Color;

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

  void print_color(const Color color) {
    switch (color.color_space) {
      case CS_RGB:
        printf("%d, %d, %d\n", (uint8_t)(color.r * 255),
               (uint8_t)(color.g * 255), (uint8_t)(color.b * 255));
        break;
      case CS_CMYK:
        printf("%d, %d, %d, %d\n", (uint8_t)(color.c * 255),
               (uint8_t)(color.m * 255), (uint8_t)(color.y * 255),
               (uint8_t)(color.k * 255));
        break;
      case CS_HSL:
        printf("%f, %f, %f\n", color.h, color.s, color.l);
        break;
      case CS_HSV:
        printf("%f, %f, %f\n", color.h, color.s, color.v);
        break;
      case CS_LAB:
        printf("%f, %f, %f\n", color.l, color.a, color.b);
        break;
    }
  }

  Color RGB(double r, double g, double b) {
    return (Color){CS_RGB, r,   g,   b,   1.0, 0.0, 0.0,
                   0.0,    0.0, 0.0, 0.0, 0.0, 0.0};
  }
  Color RGBi(uint8_t r, uint8_t g, uint8_t b) {
    return (Color){CS_RGB, r / 255.0, g / 255.0, b / 255.0, 1.0, 0.0, 0.0,
                   0.0,    0.0,       0.0,       0.0,       0.0, 0.0};
  }
  Color RGBs(const char* str) {
    bool pound = (str[0] == '#');
    char rs[3], gs[3], bs[3];
    strncpy(rs, str + pound, 2);
    strncpy(gs, str + 2 + pound, 2);
    strncpy(bs, str + 4 + pound, 2);
    return RGBi((int)strtol(rs, NULL, 16), (int)strtol(gs, NULL, 16),
                (int)strtol(bs, NULL, 16));
  }
  Color RGBA(double r, double g, double b, double a) {
    return (Color){CS_RGB, r, g, b, a, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  }
  Color RGBAi(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (Color){CS_RGB, r / 255.0, g / 255.0, b / 255.0, a / 255.0, 0.0, 0.0,
                   0.0,    0.0,       0.0,       0.0,       0.0,       0.0};
  }
  Color CMYK(double c, double m, double y, double k) {
    return (Color){CS_CMYK, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, c, m, y, k};
  }
  Color CMKYi(uint8_t c, uint8_t m, uint8_t y, uint8_t k) {
    return (Color){CS_CMYK, 0.0, 0.0,       0.0,       0.0,       0.0,      0.0,
                   0.0,     0.0, c / 255.0, m / 255.0, y / 255.0, k / 255.0};
  }
  Color HSV(double h, double s, double v) {
    h = fmod(h, 360.0);
    if (h < 0) h += 360.0;
    return (Color){CS_HSV, 0.0, 0.0, 0.0, 0.0, h,  s,
                   0.0,    v,   0.0, 0.0, 0.0, 0.0};
  }
  Color HSVi(uint16_t h, uint8_t s, uint8_t v) {
    h = fmod(h, 360);
    return (Color){CS_HSV, 0.0,       0.0, 0.0, 0.0, (double)h, s / 255.0,
                   0.0,    v / 255.0, 0.0, 0.0, 0.0, 0.0};
  }
  Color HSL(double h, double s, double l) {
    h = fmod(h, 360.0);
    if (h < 0) h += 360.0;
    return (Color){CS_HSL, 0.0, 0.0, 0.0, 0.0, h,  s,
                   l,      0.0, 0.0, 0.0, 0.0, 0.0};
  }
  Color HSLi(uint16_t h, uint8_t s, uint8_t l) {
    h = fmod(h, 360);
    return (Color){CS_HSL,    0.0, 0.0, 0.0, 0.0, (double)h, s / 255.0,
                   l / 255.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  }
  Color LAB(double l, double a, double b) {
    return (Color){CS_LAB, 0.0, 0.0, b,   a,   0.0, 0.0,
                   l,      0.0, 0.0, 0.0, 0.0, 0.0};
  }

  Color get_RGB(const Color input) {
    switch (input.color_space) {
      case CS_RGB: {
        return input;
      }
      case CS_CMYK: {
        return RGB((1.0 - input.c) * (1.0 - input.k),
                   (1.0 - input.m) * (1.0 - input.k),
                   (1.0 - input.y) * (1.0 - input.k));
      }
      case CS_HSV: {
        double h = input.h;
        double c = input.v * input.s;
        double x = c * (1.0 - fabs(fmod(h / 60.0, 2.0) - 1.0));
        double m = input.v - c;
        if (h < 60.0) {
          return RGB(c + m, x + m, +m);
        } else if (h < 120.0) {
          return RGB(x + m, c + m, +m);
        } else if (h < 180.0) {
          return RGB(+m, c + m, x + m);
        } else if (h < 240.0) {
          return RGB(+m, x + m, c + m);
        } else if (h < 300.0) {
          return RGB(x + m, +m, c + m);
        } else {
          return RGB(c + m, +m, x + m);
        }
      }
      case CS_HSL: {
        double h = input.h;
        double c = (1.0 - fabs(2 * input.l - 1.0)) * input.s;
        double x = c * (1.0 - fabs(fmod(h / 60.0, 2.0) - 1.0));
        double m = input.l - c / 2.0;
        if (h < 60.0) {
          return RGB(c + m, x + m, m);
        } else if (h < 120.0) {
          return RGB(x + m, c + m, m);
        } else if (h < 180.0) {
          return RGB(m, c + m, x + m);
        } else if (h < 240.0) {
          return RGB(m, x + m, c + m);
        } else if (h < 300.0) {
          return RGB(x + m, m, c + m);
        } else {
          return RGB(c + m, m, x + m);
        }
        return input;
      }
      case CS_LAB: {
        double y = (input.l + 16) / 116;
        double x = input.a / 500 + y;
        double z = y - input.b / 200;
        double r, g, b;
        x = 0.95047 *
            ((x * x * x > 0.008856) ? x * x * x : (x - 16 / 116) / 7.787);
        y = 1.00000 *
            ((y * y * y > 0.008856) ? y * y * y : (y - 16 / 116) / 7.787);
        z = 1.08883 *
            ((z * z * z > 0.008856) ? z * z * z : (z - 16 / 116) / 7.787);

        r = x * 3.2406 + y * -1.5372 + z * -0.4986;
        g = x * -0.9689 + y * 1.8758 + z * 0.0415;
        b = x * 0.0557 + y * -0.2040 + z * 1.0570;

        r = (r > 0.0031308) ? (1.055 * pow(r, 1 / 2.4) - 0.055) : 12.92 * r;
        g = (g > 0.0031308) ? (1.055 * pow(g, 1 / 2.4) - 0.055) : 12.92 * g;
        b = (b > 0.0031308) ? (1.055 * pow(b, 1 / 2.4) - 0.055) : 12.92 * b;
        return RGB(r, g, b);
      }
      default:
        return input;
    }
  }

  Color get_CMYK(Color input) {
    switch (input.color_space) {
      case CS_RGB: {
        double k = 1.0 - fmax(input.r, fmax(input.g, input.b));
        double c = (1.0 - input.r - k) / (1.0 - k);
        double m = (1.0 - input.g - k) / (1.0 - k);
        double y = (1.0 - input.b - k) / (1.0 - k);
        return CMYK(c, m, y, k);
      }
      case CS_CMYK: {
        return input;
      }
      default: { return get_CMYK(get_RGB(input)); }
    }
  }

  Color get_HSV(Color input) {
    switch (input.color_space) {
      case CS_RGB: {
        double c_max = fmax(input.r, fmax(input.g, input.b));
        double c_min = fmin(input.r, fmin(input.g, input.b));
        double delta = c_max - c_min;
        double h = 0, s = 0, v = 0;
        if (delta == 0) {
          h = 0;
        } else if (c_max == input.r) {
          h = 60 * fmod((input.g - input.b) / delta, 6.0);
        } else if (c_max == input.g) {
          h = 60 * ((input.b - input.r) / delta + 2);
        } else if (c_max == input.b) {
          h = 60 * ((input.r - input.g) / delta + 4);
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
        return HSV(h, s, v);
      }
      case CS_HSV: {
        return input;
      }
      default: { return get_HSV(get_RGB(input)); }
    }
  }

  Color get_HSL(Color input) {
    switch (input.color_space) {
      case CS_RGB: {
        double c_max = fmax(input.r, fmax(input.g, input.b));
        double c_min = fmin(input.r, fmin(input.g, input.b));
        double delta = c_max - c_min;
        double h = 0, s = 0, l = 0;
        if (delta == 0) {
          h = 0;
        } else if (c_max == input.r) {
          h = 60 * fmod((input.g - input.b) / delta, 6.0);
        } else if (c_max == input.g) {
          h = 60 * ((input.b - input.r) / delta + 2);
        } else if (c_max == input.b) {
          h = 60 * ((input.r - input.g) / delta + 4);
        }
        if (h < 0) {
          h += 360.0;
        }
        l = (c_max + c_min) / 2;
        if (c_max == 0) {
          s = 0;
        } else {
          s = delta / (1.0 - fabs(2 * l - 1.0));
        }
        return HSL(h, s, l);
      }
      case CS_HSL: {
        return input;
      }
      default: { return get_HSL(get_RGB(input)); }
    }
  }

  Color get_LAB(Color input) {
    switch (input.color_space) {
      case CS_RGB: {
        double r = input.r, g = input.g, b = input.b;
        double x, y, z;
        r = (r > 0.04045) ? pow((r + 0.055) / 1.055, 2.4) : r / 12.92;
        g = (g > 0.04045) ? pow((g + 0.055) / 1.055, 2.4) : r / 12.92;
        b = (b > 0.04045) ? pow((b + 0.055) / 1.055, 2.4) : r / 12.92;
        x = (r * 0.4124 + g * 0.3576 + b * 0.1805) / 0.95047;
        y = (r * 0.2126 + g * 0.7152 + b * 0.0722) / 1.00000;
        z = (r * 0.0193 + g * 0.1192 + b * 0.9505) / 1.08883;
        x = (x > 0.008856) ? pow(x, 1.0 / 3.0) : (7.787 * x) + 16.0 / 116.0;
        y = (x > 0.008856) ? pow(y, 1.0 / 3.0) : (7.787 * y) + 16.0 / 116.0;
        z = (x > 0.008856) ? pow(z, 1.0 / 3.0) : (7.787 * z) + 16.0 / 116.0;
        return LAB((116.0 * y) - 16.0, 500 * (x - y), 200 * (y - z));
      }
      case CS_LAB: {
        return input;
      }
      default: { return get_LAB(get_RGB(input)); }
    }
  }

  Color average_LAB(Color lhs, Color rhs) {
    lhs = get_LAB(lhs);
    rhs = get_LAB(rhs);
    return LAB((lhs.l + rhs.l) / 2.0, (lhs.a + rhs.a) / 2, (lhs.b + rhs.b) / 2);
  }

  Color gradient_RGB(Color start, Color end, double t) {
    start = get_RGB(start);
    end = get_RGB(end);
    double dr = end.r - start.r;
    double dg = end.g - start.g;
    double db = end.b - start.b;
    return RGB((dr * t) + start.r, (dg * t) + start.g, (db * t) + start.b);
  }
  Color gradient_Multi_RGB(Color* colors, size_t count, double t) {
    double step = 1.0 / (count - 1);
    size_t index = t / step;
    t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
    return gradient_RGB(colors[index], colors[index + 1], t);
  }
  Color gradient_CMYK(Color start, Color end, double t) {
    start = get_CMYK(start);
    end = get_CMYK(end);
    double dc = end.c - start.c;
    double dm = end.m - start.m;
    double dy = end.y - start.y;
    double dk = end.k - start.k;
    return CMYK((dc * t) + start.c, (dm * t) + start.m, (dy * t) + start.y,
                (dk * t) + start.k);
  }
  Color gradient_multi_CMYK(Color* colors, size_t count, double t) {
    double step = 1.0 / (count - 1);
    size_t index = t / step;
    t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
    return gradient_CMYK(colors[index], colors[index + 1], t);
  }
  Color gradient_HSV(Color start, Color end, double t) {
    start = get_HSV(start);
    end = get_HSV(end);
    double dh = angle_diff_min(start.h, end.h);
    double ds = end.s - start.s;
    double dv = end.v - start.v;
    return HSV((dh * t) + start.h, (ds * t) + start.s, (dv * t) + start.v);
  }
  Color gradient_multi_HSV(Color* colors, size_t count, double t) {
    double step = 1.0 / (count - 1);
    size_t index = t / step;
    t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
    return gradient_HSV(colors[index], colors[index + 1], t);
  }
  Color gradient_HSVB(Color start, Color end, double t) {
    start = get_HSV(start);
    end = get_HSV(end);
    double dh = angle_diff_max(start.h, end.h);
    double ds = end.s - start.s;
    double dv = end.v - start.v;
    return HSV((dh * t) + start.h, (ds * t) + start.s, (dv * t) + start.v);
  }
  Color gradientMultiHSVB(Color* colors, size_t count, double t) {
    double step = 1.0 / (count - 1);
    size_t index = t / step;
    t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
    return gradient_HSVB(colors[index], colors[index + 1], t);
  }
  Color gradient_HSV_clock(Color start, Color end, double t) {
    start = get_HSV(start);
    end = get_HSV(end);
    double dh = angle_diff_clockwise(start.h, end.h);
    double ds = end.s - start.s;
    double dv = end.v - start.v;
    return HSV((dh * t) + start.h, (ds * t) + start.s, (dv * t) + start.v);
  }
  Color gradient_multi_HSV_clock(Color* colors, size_t count, double t) {
    double step = 1.0 / (count - 1);
    size_t index = t / step;
    t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
    return gradient_HSV_clock(colors[index], colors[index + 1], t);
  }
  Color gradient_HSV_counter(Color start, Color end, double t) {
    start = get_HSV(start);
    end = get_HSV(end);
    double dh = angle_diff_counter_clockwise(start.h, end.h);
    double ds = end.s - start.s;
    double dv = end.v - start.v;
    return HSV((dh * t) + start.h, (ds * t) + start.s, (dv * t) + start.v);
  }
  Color gradientMultiHSVCounter(Color* colors, size_t count, double t) {
    double step = 1.0 / (count - 1);
    size_t index = t / step;
    t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
    return gradient_HSV_counter(colors[index], colors[index + 1], t);
  }
  Color gradient_HSL(Color start, Color end, double t) {
    start = get_HSL(start);
    end = get_HSL(end);
    double dh = angle_diff_min(start.h, end.h);
    double ds = end.s - start.s;
    double dl = end.l - start.l;
    return HSL((dh * t) + start.h, (ds * t) + start.s, (dl * t) + start.l);
  }
  Color gradient_multi_HSL(Color* colors, size_t count, double t) {
    double step = 1.0 / (count - 1);
    size_t index = t / step;
    t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
    return gradient_HSL(colors[index], colors[index + 1], t);
  }
  Color gradient_HSLB(Color start, Color end, double t) {
    start = get_HSL(start);
    end = get_HSL(end);
    double dh = angle_diff_max(start.h, end.h);
    double ds = end.s - start.s;
    double dl = end.l - start.l;
    return HSL((dh * t) + start.h, (ds * t) + start.s, (dl * t) + start.l);
  }
  Color gradient_multi_HSLB(Color* colors, size_t count, double t) {
    double step = 1.0 / (count - 1);
    size_t index = t / step;
    t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
    return gradient_HSLB(colors[index], colors[index + 1], t);
  }
  Color gradient_HSL_clock(Color start, Color end, double t) {
    start = get_HSL(start);
    end = get_HSL(end);
    double dh = angle_diff_clockwise(start.h, end.h);
    double ds = end.s - start.s;
    double dl = end.l - start.l;
    return HSL((dh * t) + start.h, (ds * t) + start.s, (dl * t) + start.l);
  }
  Color gradient_multi_HSL_clock(Color* colors, size_t count, double t) {
    double step = 1.0 / (count - 1);
    size_t index = t / step;
    t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
    return gradient_HSL_clock(colors[index], colors[index + 1], t);
  }
  Color gradient_HSL_counter(Color start, Color end, double t) {
    start = get_HSL(start);
    end = get_HSL(end);
    double dh = angle_diff_counter_clockwise(start.h, end.h);
    double ds = end.s - start.s;
    double dl = end.l - start.l;
    return HSL((dh * t) + start.h, (ds * t) + start.s, (dl * t) + start.l);
  }
  Color gradient_multi_HSL_counter(Color* colors, size_t count, double t) {
    double step = 1.0 / (count - 1);
    size_t index = t / step;
    t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
    return gradient_HSL_counter(colors[index], colors[index + 1], t);
  }
  Color gradient_LAB(Color start, Color end, double t) {
    start = get_LAB(start);
    end = get_LAB(end);
    double dl = end.l - start.l;
    double da = end.a - start.a;
    double db = end.b - start.b;
    return LAB((dl * t) + start.l, (da * t) + start.a, (db * t) + start.b);
  }
  Color gradient_multi_LAB(Color* colors, size_t count, double t) {
    double step = 1.0 / (count - 1);
    size_t index = t / step;
    t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
    return gradient_LAB(colors[index], colors[index + 1], t);
  }

  Color gradient_2D(Color tl, Color tr, Color bl, Color br,
                   Color (*gradient)(Color, Color, double), double x,
                   double y) {
    // return gradient(gradient(tl, tr, x), gradient(bl, br, x), y);
    return gradient(gradient(tl, bl, y), gradient(tr, br, y), x);
  }
  Color gradient_2DB(Color tl, Color tr, Color bl, Color br, Color c,
                    Color (*gradient)(Color, Color, double), double x,
                    double y) {
    if (y < 0.5) {
      if (x < 0.5) {
        return gradient_2D(tl, gradient(tl, tr, 0.5), gradient(tl, bl, 0.5), c,
                          gradient, x * 2.0, y * 2.0);
      } else if (x > 0.5) {
        return gradient_2D(gradient(tl, tr, 0.5), tr, c, gradient(tr, br, 0.5),
                          gradient, (x - 0.5) * 2.0, y * 2.0);
      } else {
        return gradient(gradient(tl, tr, 0.5), c, y * 2.0);
      }
    } else if (y > 0.5) {
      if (x < 0.5) {
        return gradient_2D(gradient(tl, bl, 0.5), c, bl, gradient(bl, br, 0.5),
                          gradient, x * 2.0, (y - 0.5) * 2.0);
      } else if (x > 0.5) {
        return gradient_2D(c, gradient(tr, br, 0.5), gradient(bl, br, 0.5), br,
                          gradient, (x - 0.5) * 2.0, (y - 0.5) * 2.0);
      } else {
        return gradient(c, gradient(bl, br, 0.5), (y - 0.5) * 2.0);
      }
    } else {
      if (x < 0.5) {
        return gradient(gradient(tl, bl, 0.5), c, x * 2.0);
      } else if (x > 0.5) {
        return gradient(c, gradient(tr, br, 0.5), (x - 0.5) * 2.0);
      } else {
        return c;
      }
    }
  }
  Color gradient_2DC(Color tl, Color tc, Color tr, Color cl, Color cc, Color cr,
                    Color bl, Color bc, Color br,
                    Color (*gradient)(Color, Color, double), double x,
                    double y) {
    if (y < 0.5) {
      if (x < 0.5) {
        return gradient_2D(tl, tc, cl, cc, gradient, x * 2.0, y * 2.0);
      } else if (x > 0.5) {
        return gradient_2D(tc, tr, cc, cr, gradient, (x - 0.5) * 2.0, y * 2.0);
      } else {
        return gradient(tc, cc, y * 2.0);
      }
    } else if (y > 0.5) {
      if (x < 0.5) {
        return gradient_2D(cl, cc, bl, bc, gradient, x * 2.0, (y - 0.5) * 2.0);
      } else if (x > 0.5) {
        return gradient_2D(cc, cr, bc, br, gradient, (x - 0.5) * 2.0,
                          (y - 0.5) * 2.0);
      } else {
        return gradient(cc, bc, (y - 0.5) * 2.0);
      }
    } else {
      if (x < 0.5) {
        return gradient(cl, cc, x * 2.0);
      } else if (x > 0.5) {
        return gradient(cc, cr, (x - 0.5) * 2.0);
      } else {
        return cc;
      }
    }
  }

  void print_escape(const Color color) {
    Color rgb = get_RGB(color);
    printf("\033[38;2;%d;%d;%dm", (int)(rgb.r * 255), (int)(rgb.g * 255),
           (int)(rgb.b * 255));
  }

#ifdef __cplusplus
  std::ostream& operator<<(std::ostream& out, const Color& lhs) {
    switch (lhs.color_space) {
      case CS_RGB:
        out << (uint8_t)(lhs.r * 255) << ", " << (uint8_t)(lhs.g * 255) << ", "
            << (uint8_t)(lhs.b * 255);
        break;
      case CS_CMYK:
        out << lhs.c << ", " << lhs.m << ", " << lhs.y << ", " << lhs.k;
        break;
      case CS_HSL:
        out << lhs.h << ", " << lhs.s << ", " << lhs.l;
        break;
      case CS_HSV:
        out << lhs.h << ", " << lhs.s << ", " << lhs.v;
        break;
      case CS_LAB:
        out << lhs.l << ", " << lhs.a << ", " << lhs.b;
        break;
    }
    return out;
  }
#endif

#ifdef __cplusplus
}  // namespace tingo
}
#endif

#endif  // TINGO_H_
