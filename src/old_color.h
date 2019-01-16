#ifndef TINGO_H_
#define TINGO_H_

#include <cmath>
#include <cstdbool>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

typedef enum { CS_RGB, CS_CMYK, CS_HSV, CS_HSL, CS_LAB } ColorSpace;
typedef struct Color {
  ColorSpace color_space;
  double r, g, b, a;
  double h, s, l, v;
  double c, m, y, k;
} Color;

double AngleDiffClockwise(double start, double end) {
  if (start <= end) {
    return end - start;
  } else {
    return 360.0 + end - start;
  }
}
double AngleDiffCounterClockwise(double start, double end) {
  if (end <= start) {
    return end - start;
  } else {
    return -360.0 - start + end;
  }
}
double AngleDiffMin(double start, double end) {
  double cw = AngleDiffClockwise(start, end);
  double ccw = AngleDiffCounterClockwise(start, end);
  if (fabs(cw) > fabs(ccw)) {
    return ccw;
  } else {
    return cw;
  }
}
double AngleDiffMax(double start, double end) {
  double cw = AngleDiffClockwise(start, end);
  double ccw = AngleDiffCounterClockwise(start, end);
  if (fabs(cw) > fabs(ccw)) {
    return cw;
  } else {
    return ccw;
  }
}

void PrintColor(const Color color) {
  switch (color.color_space) {
    case CS_RGB:
      printf("%d, %d, %d\n", (uint8_t)(color.r * 255), (uint8_t)(color.g * 255),
             (uint8_t)(color.b * 255));
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

Color INT(uint32_t hex) {
  return (Color){CS_RGB,
                 static_cast<double>((hex >> 16) & 0xFF) / 255.0,
                 static_cast<double>((hex >> 8) & 0xFF) / 255.0,
                 static_cast<double>((hex)&0XFF) / 255.0,
                 1.0,
                 0.0,
                 0.0,
                 0.0,
                 0.0,
                 0.0,
                 0.0,
                 0.0,
                 0.0};
}
Color RGB(double r, double g, double b) {
  return (Color){CS_RGB, r, g, b, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
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
  return (Color){CS_HSV, 0.0, 0.0, 0.0, 0.0, h, s, 0.0, v, 0.0, 0.0, 0.0, 0.0};
}
Color HSVi(uint16_t h, uint8_t s, uint8_t v) {
  h = fmod(h, 360);
  return (Color){CS_HSV, 0.0,       0.0, 0.0, 0.0, (double)h, s / 255.0,
                 0.0,    v / 255.0, 0.0, 0.0, 0.0, 0.0};
}
Color HSL(double h, double s, double l) {
  h = fmod(h, 360.0);
  if (h < 0) h += 360.0;
  return (Color){CS_HSL, 0.0, 0.0, 0.0, 0.0, h, s, l, 0.0, 0.0, 0.0, 0.0, 0.0};
}
Color HSLi(uint16_t h, uint8_t s, uint8_t l) {
  h = fmod(h, 360);
  return (Color){CS_HSL,    0.0, 0.0, 0.0, 0.0, (double)h, s / 255.0,
                 l / 255.0, 0.0, 0.0, 0.0, 0.0, 0.0};
}
Color LAB(double l, double a, double b) {
  return (Color){CS_LAB, 0.0, 0.0, b, a, 0.0, 0.0, l, 0.0, 0.0, 0.0, 0.0, 0.0};
}

Color GetRGB(const Color input) {
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

Color GetCMYK(Color input) {
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
    default: { return GetCMYK(GetRGB(input)); }
  }
}

Color GetHSV(Color input) {
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
    default: { return GetHSV(GetRGB(input)); }
  }
}

Color GetHSL(Color input) {
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
    default: { return GetHSL(GetRGB(input)); }
  }
}

Color GetLAB(Color input) {
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
    default: { return GetLAB(GetRGB(input)); }
  }
}

Color AverageLAB(Color lhs, Color rhs) {
  lhs = GetLAB(lhs);
  rhs = GetLAB(rhs);
  return LAB((lhs.l + rhs.l) / 2.0, (lhs.a + rhs.a) / 2, (lhs.b + rhs.b) / 2);
}

Color GradientRGB(Color start, Color end, double t) {
  start = GetRGB(start);
  end = GetRGB(end);
  double dr = end.r - start.r;
  double dg = end.g - start.g;
  double db = end.b - start.b;
  return RGB((dr * t) + start.r, (dg * t) + start.g, (db * t) + start.b);
}
Color GradientMultiRGB(Color* colors, size_t count, double t) {
  double step = 1.0 / (count - 1);
  size_t index = t / step;
  t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
  return GradientRGB(colors[index], colors[index + 1], t);
}
Color GradientCMYK(Color start, Color end, double t) {
  start = GetCMYK(start);
  end = GetCMYK(end);
  double dc = end.c - start.c;
  double dm = end.m - start.m;
  double dy = end.y - start.y;
  double dk = end.k - start.k;
  return CMYK((dc * t) + start.c, (dm * t) + start.m, (dy * t) + start.y,
              (dk * t) + start.k);
}
Color GradientMultiCMYK(Color* colors, size_t count, double t) {
  double step = 1.0 / (count - 1);
  size_t index = t / step;
  t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
  return GradientCMYK(colors[index], colors[index + 1], t);
}
Color GradientHSV(Color start, Color end, double t) {
  start = GetHSV(start);
  end = GetHSV(end);
  double dh = AngleDiffMin(start.h, end.h);
  double ds = end.s - start.s;
  double dv = end.v - start.v;
  return HSV((dh * t) + start.h, (ds * t) + start.s, (dv * t) + start.v);
}
Color GradientMultiHSV(Color* colors, size_t count, double t) {
  double step = 1.0 / (count - 1);
  size_t index = t / step;
  t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
  return GradientHSV(colors[index], colors[index + 1], t);
}
Color GradientHSVB(Color start, Color end, double t) {
  start = GetHSV(start);
  end = GetHSV(end);
  double dh = AngleDiffMax(start.h, end.h);
  double ds = end.s - start.s;
  double dv = end.v - start.v;
  return HSV((dh * t) + start.h, (ds * t) + start.s, (dv * t) + start.v);
}
Color GradientMultiHSVB(Color* colors, size_t count, double t) {
  double step = 1.0 / (count - 1);
  size_t index = t / step;
  t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
  return GradientHSVB(colors[index], colors[index + 1], t);
}
Color GradientHSVClock(Color start, Color end, double t) {
  start = GetHSV(start);
  end = GetHSV(end);
  double dh = AngleDiffClockwise(start.h, end.h);
  double ds = end.s - start.s;
  double dv = end.v - start.v;
  return HSV((dh * t) + start.h, (ds * t) + start.s, (dv * t) + start.v);
}
Color GradientMultiHSVClock(Color* colors, size_t count, double t) {
  double step = 1.0 / (count - 1);
  size_t index = t / step;
  t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
  return GradientHSVClock(colors[index], colors[index + 1], t);
}
Color GradientHSVCounter(Color start, Color end, double t) {
  start = GetHSV(start);
  end = GetHSV(end);
  double dh = AngleDiffCounterClockwise(start.h, end.h);
  double ds = end.s - start.s;
  double dv = end.v - start.v;
  return HSV((dh * t) + start.h, (ds * t) + start.s, (dv * t) + start.v);
}
Color GradientMultiHSVCounter(Color* colors, size_t count, double t) {
  double step = 1.0 / (count - 1);
  size_t index = t / step;
  t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
  return GradientHSVCounter(colors[index], colors[index + 1], t);
}
Color GradientHSL(Color start, Color end, double t) {
  start = GetHSL(start);
  end = GetHSL(end);
  double dh = AngleDiffMin(start.h, end.h);
  double ds = end.s - start.s;
  double dl = end.l - start.l;
  return HSL((dh * t) + start.h, (ds * t) + start.s, (dl * t) + start.l);
}
Color GradientMultiHSL(Color* colors, size_t count, double t) {
  double step = 1.0 / (count - 1);
  size_t index = t / step;
  t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
  return GradientHSL(colors[index], colors[index + 1], t);
}
Color GradientHSLB(Color start, Color end, double t) {
  start = GetHSL(start);
  end = GetHSL(end);
  double dh = AngleDiffMax(start.h, end.h);
  double ds = end.s - start.s;
  double dl = end.l - start.l;
  return HSL((dh * t) + start.h, (ds * t) + start.s, (dl * t) + start.l);
}
Color GradientMultiHSLB(Color* colors, size_t count, double t) {
  double step = 1.0 / (count - 1);
  size_t index = t / step;
  t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
  return GradientHSLB(colors[index], colors[index + 1], t);
}
Color GradientHSLClock(Color start, Color end, double t) {
  start = GetHSL(start);
  end = GetHSL(end);
  double dh = AngleDiffClockwise(start.h, end.h);
  double ds = end.s - start.s;
  double dl = end.l - start.l;
  return HSL((dh * t) + start.h, (ds * t) + start.s, (dl * t) + start.l);
}
Color GradientMultiHSLClock(Color* colors, size_t count, double t) {
  double step = 1.0 / (count - 1);
  size_t index = t / step;
  t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
  return GradientHSLClock(colors[index], colors[index + 1], t);
}
Color GradientHSLCounter(Color start, Color end, double t) {
  start = GetHSL(start);
  end = GetHSL(end);
  double dh = AngleDiffCounterClockwise(start.h, end.h);
  double ds = end.s - start.s;
  double dl = end.l - start.l;
  return HSL((dh * t) + start.h, (ds * t) + start.s, (dl * t) + start.l);
}
Color GradientMultiHSLCounter(Color* colors, size_t count, double t) {
  double step = 1.0 / (count - 1);
  size_t index = t / step;
  t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
  return GradientHSLCounter(colors[index], colors[index + 1], t);
}
Color GradientLAB(Color start, Color end, double t) {
  start = GetLAB(start);
  end = GetLAB(end);
  double dl = end.l - start.l;
  double da = end.a - start.a;
  double db = end.b - start.b;
  return LAB((dl * t) + start.l, (da * t) + start.a, (db * t) + start.b);
}
Color GradientMultiLAB(Color* colors, size_t count, double t) {
  double step = 1.0 / (count - 1);
  size_t index = t / step;
  t = (1.0 / (((index + 1) * step) - (index * step))) * (t - (index * step));
  return GradientLAB(colors[index], colors[index + 1], t);
}

Color Gradient2D(Color tl, Color tr, Color bl, Color br,
                 Color (*Gradient)(Color, Color, double), double x, double y) {
  // return Gradient(Gradient(tl, tr, x), Gradient(bl, br, x), y);
  return Gradient(Gradient(tl, bl, y), Gradient(tr, br, y), x);
}
Color Gradient2DB(Color tl, Color tr, Color bl, Color br, Color c,
                  Color (*Gradient)(Color, Color, double), double x, double y) {
  if (y < 0.5) {
    if (x < 0.5) {
      return Gradient2D(tl, Gradient(tl, tr, 0.5), Gradient(tl, bl, 0.5), c,
                        Gradient, x * 2.0, y * 2.0);
    } else if (x > 0.5) {
      return Gradient2D(Gradient(tl, tr, 0.5), tr, c, Gradient(tr, br, 0.5),
                        Gradient, (x - 0.5) * 2.0, y * 2.0);
    } else {
      return Gradient(Gradient(tl, tr, 0.5), c, y * 2.0);
    }
  } else if (y > 0.5) {
    if (x < 0.5) {
      return Gradient2D(Gradient(tl, bl, 0.5), c, bl, Gradient(bl, br, 0.5),
                        Gradient, x * 2.0, (y - 0.5) * 2.0);
    } else if (x > 0.5) {
      return Gradient2D(c, Gradient(tr, br, 0.5), Gradient(bl, br, 0.5), br,
                        Gradient, (x - 0.5) * 2.0, (y - 0.5) * 2.0);
    } else {
      return Gradient(c, Gradient(bl, br, 0.5), (y - 0.5) * 2.0);
    }
  } else {
    if (x < 0.5) {
      return Gradient(Gradient(tl, bl, 0.5), c, x * 2.0);
    } else if (x > 0.5) {
      return Gradient(c, Gradient(tr, br, 0.5), (x - 0.5) * 2.0);
    } else {
      return c;
    }
  }
}
Color Gradient2DC(Color tl, Color tc, Color tr, Color cl, Color cc, Color cr,
                  Color bl, Color bc, Color br,
                  Color (*Gradient)(Color, Color, double), double x, double y) {
  if (y < 0.5) {
    if (x < 0.5) {
      return Gradient2D(tl, tc, cl, cc, Gradient, x * 2.0, y * 2.0);
    } else if (x > 0.5) {
      return Gradient2D(tc, tr, cc, cr, Gradient, (x - 0.5) * 2.0, y * 2.0);
    } else {
      return Gradient(tc, cc, y * 2.0);
    }
  } else if (y > 0.5) {
    if (x < 0.5) {
      return Gradient2D(cl, cc, bl, bc, Gradient, x * 2.0, (y - 0.5) * 2.0);
    } else if (x > 0.5) {
      return Gradient2D(cc, cr, bc, br, Gradient, (x - 0.5) * 2.0,
                        (y - 0.5) * 2.0);
    } else {
      return Gradient(cc, bc, (y - 0.5) * 2.0);
    }
  } else {
    if (x < 0.5) {
      return Gradient(cl, cc, x * 2.0);
    } else if (x > 0.5) {
      return Gradient(cc, cr, (x - 0.5) * 2.0);
    } else {
      return cc;
    }
  }
}

void PrintEscape(const Color color) {
  Color rgb = GetRGB(color);
  printf("\033[38;2;%d;%d;%dm", (int)(rgb.r * 255), (int)(rgb.g * 255),
         (int)(rgb.b * 255));
}

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

#endif  // TINGO_H_
