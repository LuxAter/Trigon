#ifndef TRIGON_COLOR_HPP_
#define TRIGON_COLOR_HPP_

#include <cstdint>
#include <string>
#include <vector>

enum ColorSpace { RGB, CMYK, HSV, HSL };

uint32_t rgb(const double& r, const double& g, const double& b);
uint32_t rgb(const uint8_t& r, const uint8_t& g, const uint8_t& b);
uint32_t rgb(const std::string& hex);
uint32_t rgb(const std::vector<double>& c);
uint32_t rgb(const std::vector<uint8_t>& c);
std::vector<double> get_rgb(const uint32_t& c);
std::vector<uint8_t> get_rgbi(const uint32_t& c);

uint32_t cmyk(const double& c, const double& m, const double& y,
              const double& k);
uint32_t cmyk(const uint8_t& c, const uint8_t& m, const uint8_t& y,
              const uint8_t& k);
uint32_t cmyk(const std::vector<double>& c);
uint32_t cmyk(const std::vector<uint8_t>& c);
std::vector<double> get_cmyk(const uint32_t& c);
std::vector<uint8_t> get_cmyki(const uint32_t& c);

uint32_t hsv(const double& h, const double& s, const double& v);
uint32_t hsv(const std::vector<double>& c);
std::vector<double> get_hsv(const uint32_t& c);

uint32_t hsl(const double& h, const double& s, const double& l);
uint32_t hsl(const std::vector<double>& c);
std::vector<double> get_hsl(const uint32_t& c);

uint32_t grad(const double& t, const uint32_t& a, const uint32_t& b,
              ColorSpace method = HSV);
uint32_t grad(const double& t, std::vector<uint32_t> colors,
              ColorSpace method = HSV);

uint32_t grad_rgb(const double& t, const uint32_t& a, const uint32_t& b);
uint32_t grad_rgb(const double& t, std::vector<uint32_t> colors);

uint32_t grad_cmyk(const double& t, const uint32_t& a, const uint32_t& b);
uint32_t grad_cmyk(const double& t, std::vector<uint32_t> colors);

uint32_t grad_hsv(const double& t, const uint32_t& a, const uint32_t& b);
uint32_t grad_hsv(const double& t, std::vector<uint32_t> colors);

uint32_t grad_hsl(const double& t, const uint32_t& a, const uint32_t& b);
uint32_t grad_hsl(const double& t, std::vector<uint32_t> colors);

#endif  // TRIGON_COLOR_HPP_
