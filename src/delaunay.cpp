#include "delaunay.h"

#include <array>
#include <random>
#include <vector>
#include <stack>

std::vector<std::array<double, 2>> GenPoints(double variance, double size) {
  std::random_device rd{};
  std::mt19937 gen{rd()};
  std::uniform_real_distribution<double> dist(0.0, variance * size);
  std::vector<std::array<double, 2>> points;
  for (double y = 0.0 - size; y <= 1.0 + 2*size; y += size) {
    for (double x = 0.0 - size; x <= 1.0 + 2*size; x += size) {
      points.push_back({{x + dist(gen), y + dist(gen)}});
    }
  }
  return points;
}

std::vector<std::array<uint64_t, 3>> DelTri(
    std::vector<std::array<double, 2>> points) {
  return Delaun(points).first;
}

std::pair<std::vector<std::array<uint64_t, 3>>,
          std::vector<std::array<uint64_t, 3>>>
Delaun(std::vector<std::array<double, 2>> points) {
  uint64_t num_points = points.size();
  points.push_back({{-100, -100}});
  points.push_back({{100, -100}});
  points.push_back({{0, 100}});
  std::vector<std::array<uint64_t, 3>> vrt, adj;
  vrt.push_back({{0, 0, 0}});
  adj.push_back({{0, 0, 0}});
  vrt.push_back({{points.size() - 3, points.size() - 2, points.size() - 1}});
  adj.push_back({{0, 0, 0}});
  for (uint64_t i = 0; i < points.size() - 3; ++i) {
    uint64_t tri = TriLoc(points[i], points, vrt, adj);

    uint64_t a = adj[tri][0];
    uint64_t b = adj[tri][1];
    uint64_t c = adj[tri][2];
    uint64_t v1 = vrt[tri][0];
    uint64_t v2 = vrt[tri][1];
    uint64_t v3 = vrt[tri][2];

    vrt[tri] = {{i, v1, v2}};
    adj[tri] = {{vrt.size() + 1, a, vrt.size()}};

    vrt.push_back({{i, v2, v3}});
    adj.push_back({{tri, b, vrt.size()}});
    vrt.push_back({{i, v3, v1}});
    adj.push_back({{vrt.size() - 2, c, tri}});

    std::stack<uint64_t> tri_stack;

    if (a != 0) {
      tri_stack.push(tri);
    }
    if (b != 0) {
      adj[b][Edg(b, tri, adj)] = vrt.size() - 2;
      tri_stack.push(vrt.size() - 2);
    }
    if (c != 0) {
      adj[c][Edg(c, tri, adj)] = vrt.size() - 1;
      tri_stack.push(vrt.size() - 1);
    }

    while (tri_stack.empty() == false) {
      uint64_t l = tri_stack.top();
      tri_stack.pop();
      uint64_t r = adj[l][1];

      uint64_t erl = Edg(r, l, adj);
      uint64_t era = (erl + 1) % 3;
      uint64_t erb = (era + 1) % 3;
      v1 = vrt[r][erl];
      v2 = vrt[r][era];
      v3 = vrt[r][erb];

      if (Swap(points[v1], points[v2], points[v3], points[i]) == true) {
        a = adj[r][era];
        b = adj[r][erb];
        c = adj[l][2];

        vrt[l][2] = v3;
        adj[l][1] = a;
        adj[l][2] = r;

        vrt[r][0] = i;
        vrt[r][1] = v3;
        vrt[r][2] = v1;
        adj[r][0] = l;
        adj[r][1] = b;
        adj[r][2] = c;

        if (a != 0) {
          adj[a][Edg(a, r, adj)] = l;
          tri_stack.push(l);
        }
        if (b != 0) {
          tri_stack.push(r);
        }
        if (c != 0) {
          adj[c][Edg(c, l, adj)] = r;
        }
      }
    }
  }
  uint64_t tri;
  for (tri = 0; tri < vrt.size(); ++tri) {
    if (vrt[tri][0] >= num_points || vrt[tri][1] >= num_points ||
        vrt[tri][2] >= num_points) {
      for (uint64_t i = 0; i < 3; ++i) {
        uint64_t a = adj[tri][i];
        if (a != 0) {
          adj[a][Edg(a, tri, adj)] = 0;
        }
      }
      break;
    }
  }

  uint64_t tri_start = tri + 1;
  uint64_t tri_stop = vrt.size();
  uint64_t num_tri = tri - 1;

  for (tri = tri_start; tri < tri_stop; ++tri) {
    if (vrt[tri][0] >= num_points || vrt[tri][1] >= num_points ||
        vrt[tri][2] >= num_points) {
      for (uint64_t i = 0; i < 3; ++i) {
        uint64_t a = adj[tri][i];
        if (a != 0) {
          adj[a][Edg(a, tri, adj)] = 0;
        }
      }
    } else {
      num_tri++;
      for (uint64_t i = 0; i < 3; ++i) {
        uint64_t a = adj[tri][i];
        adj[num_tri][i] = a;
        vrt[num_tri][i] = vrt[tri][i];
        if (a != 0) {
          adj[a][Edg(a, tri, adj)] = num_tri;
        }
      }
    }
  }
  vrt.resize(num_tri + 1);
  vrt.resize(num_tri + 1);
  return std::make_pair(vrt, adj);
}

uint64_t Edg(const uint64_t& tri, const uint64_t& adj,
             const std::vector<std::array<uint64_t, 3>>& edg) {
  for (uint64_t i = 0; i < 3; ++i) {
    if (edg[tri][i] == adj) {
      return i;
    }
  }
  return 0;
}

uint64_t TriLoc(const std::array<double, 2>& p,
                const std::vector<std::array<double, 2>>& points,
                const std::vector<std::array<uint64_t, 3>>& vertices,
                const std::vector<std::array<uint64_t, 3>>& adjacency) {
  uint64_t t = vertices.size() - 1;
  bool searching = true;
  while (searching == true) {
    bool within = true;
    for (uint64_t i = 0; i < 3; ++i) {
      uint64_t v1 = vertices[t][i];
      uint64_t v2 = vertices[t][(i + 1) % 3];
      if ((points[v1][1] - p[1]) * (points[v2][0] - p[0]) >
          (points[v1][0] - p[0]) * (points[v2][1] - p[1])) {
        t = adjacency[t][i];
        within = false;
      }
    }
    if (within == true) {
      searching = false;
    }
  }
  return t;
}

bool Swap(const std::array<double, 2>& p1, const std::array<double, 2>& p2,
          const std::array<double, 2>& p3, const std::array<double, 2>& p) {
  std::array<double, 2> e13 = p1 - p3;
  std::array<double, 2> e23 = p2 - p3;
  std::array<double, 2> e1p = p1 - p;
  std::array<double, 2> e2p = p2 - p;
  double cos_a = e13[0] * e23[0] + e13[1] * e23[1];
  double cos_b = e2p[0] * e1p[0] + e1p[1] * e2p[1];
  if (cos_a >= 0.0 && cos_b >= 0.0) {
    return false;
  } else if (cos_a < 0.0 && cos_b < 0.0) {
    return true;
  } else {
    double sin_a = e13[0] * e23[1] - e23[0] * e13[1];
    double sin_b = e2p[0] * e1p[1] - e1p[0] * e2p[1];
    if (sin_a * cos_b + sin_b * cos_a < 0.0) {
      return true;
    } else {
      return false;
    }
  }
}
