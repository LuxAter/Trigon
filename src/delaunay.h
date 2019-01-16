#ifndef TRIGON_DELAUNAY_H_
#define TRIGON_DELAUNAY_H_

#include <array>
#include <random>
#include <vector>

struct point {
  point(double x, double y) : x(x), y(y) {}
  double x, y;
};

struct tri {
  tri(const std::array<std::size_t, 3>& arr)
      : a(arr[0]), b(arr[1]), c(arr[2]) {}
  std::size_t a, b, c;
};

std::vector<std::array<double, 2>> GenPoints(double variance, double size);

std::vector<std::array<uint64_t, 3>> DelTri(
    std::vector<std::array<double, 2>> points);

std::pair<std::vector<std::array<uint64_t, 3>>,
          std::vector<std::array<uint64_t, 3>>>
Delaun(std::vector<std::array<double, 2>> points);
uint64_t Edg(const uint64_t& tri, const uint64_t& adj,
             const std::vector<std::array<uint64_t, 3>>& edg);
uint64_t TriLoc(const std::array<double, 2>& p,
                const std::vector<std::array<double, 2>>& points,
                const std::vector<std::array<uint64_t, 3>>& vertices,
                const std::vector<std::array<uint64_t, 3>>& adjacency);
bool Swap(const std::array<double, 2>& p1, const std::array<double, 2>& p2,
          const std::array<double, 2>& p3, const std::array<double, 2>& p);

template <typename _T>
std::array<_T, 2> operator-(const std::array<_T, 2>& lhs,
                            const std::array<_T, 2>& rhs) {
  return {{lhs[0] - rhs[0], lhs[1] - rhs[1]}};
}

#endif  // TRIGON_DELAUNAY_H_
