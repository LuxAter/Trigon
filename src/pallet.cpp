#include "pallet.h"

#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

std::vector<uint32_t> Pallet(const std::string& pallet) {
  std::vector<uint32_t> colors;
  for (std::size_t i = 0; i < pallet.size(); ++i) {
    if (abrev_.count(pallet[i]) > 0) {
      uint8_t id = 5;
      std::string ref = abrev_.at(pallet[i]);
      if (i + 1 < pallet.size() && isdigit(pallet[i + 1])) {
        id = pallet[i + 1] - 48;
        i += 1;
      }
      colors.push_back(colors_.at(ref)[id]);
    } else {
      std::cerr << "\'" << pallet[i] << "\' Is not a valid color\n";
      break;
    }
  }
  return colors;
}

std::vector<uint32_t> RandPallet() {
  unsigned nodes = (rand() % 3) + 2;
  std::vector<std::string> colors;
  std::vector<uint32_t> pallet;
  std::random_device rd{};
  std::mt19937 gen{rd()};
  std::normal_distribution<> d{4.5, 1.2};
  for (unsigned i = 0; i < nodes; ++i) {
    auto it = colors_.begin();
    std::advance(it, rand() % 16);
    pallet.push_back(it->second[std::max(
        std::min(static_cast<int>(std::round(d(gen))), 0), 8)]);
  }
  return pallet;
}
