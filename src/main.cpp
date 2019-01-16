#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include <png.h>

#include "color.h"
#include "delaunay.h"
#include "pallet.h"

unsigned img_width = 1920, img_height = 1080;
std::string output = "out";
double variance = 0.3;
double size = 0.1;
std::vector<uint32_t> pallet;
unsigned number = 1;
int full_rand = 0;

bool IsString(char* str) {
  std::string arg(str);
  return arg.find_first_of(
             "ghijklmnopqrstuvwxyzGHIJKLMNOPQRStUVWXYZ1234567890.-") !=
         std::string::npos;
}

void RandomizePallet() {
  auto it = pallets_.begin();
  std::advance(it, rand() % pallets_.size());
  pallet = Pallet(it->second);
}

void Randomize() {
  variance = (rand() / static_cast<double>(RAND_MAX));
  size =  0.5 * (rand() / static_cast<double>(RAND_MAX));
  if (full_rand == 2) {
    pallet = RandPallet();
  } else if (full_rand == 1) {
    RandomizePallet();
  }
}

void ParseArgs(int argc, char* argv[]) {
  int state = 0;
  for (int i = 1; i < argc; ++i) {
    std::string arg(argv[i]);
    switch (state) {
      case 0:
        if (arg == "-w")
          state = 1;
        else if (arg == "-h")
          state = 2;
        else if (arg == "-o")
          state = 3;
        else if (arg == "-s")
          state = 4;
        else if (arg == "-c")
          state = 5;
        else if (arg == "-r") {
          full_rand = 1;
          RandomizePallet();
        } else if (arg == "-ra") {
          Randomize();
        } else if (arg == "-R") {
          full_rand = 2;
          pallet = RandPallet();
        } else if (arg == "-n")
          state = 6;
        else
          state = 7;
        break;
      case 1:
        img_width = std::atoi(argv[i]);
        state = 0;
        break;
      case 2:
        img_height = std::atoi(argv[i]);
        state = 0;
        break;
      case 3:
        output = std::string(argv[i]);
        state = 0;
        break;
      case 4:
        size = std::atof(argv[i]);
        state = 0;
        break;
      case 5:
        if (pallets_.count(argv[i]) > 0) {
          pallet = Pallet(pallets_.at(argv[i]));
        } else {
          pallet = Pallet(argv[i]);
        }
        state = 0;
        break;
      case 6:
        number = std::atoi(argv[i]);
        state = 0;
        break;
    }
  }
  if (pallet.size() == 0) {
    RandomizePallet();
  }
}

void WritePng(const std::string& file, std::vector<uint32_t> buffer) {
  uint32_t width = img_width;
  uint32_t height = img_height;
  FILE* out = fopen(file.c_str(), "wb");
  if (!out) {
    return;
  }
  png_structp png =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) {
    fclose(out);
    return;
  }
  png_infop info = png_create_info_struct(png);
  if (!info) {
    fclose(out);
    return;
  }
  if (setjmp(png_jmpbuf(png))) {
    fclose(out);
    return;
  }
  uint8_t** byte_data = (png_bytepp)malloc(height * sizeof(png_bytep));
  for (uint32_t i = 0; i < height; ++i) {
    byte_data[i] = (png_bytep)malloc(6 * width * sizeof(png_byte));
  }
  for (uint32_t y = 0; y < height; ++y) {
    for (uint32_t x = 0; x < width; ++x) {
      uint32_t red = ((buffer[(y * width) + x] >> 16) & 0XFF) * 256;
      uint32_t green = ((buffer[(y * width) + x] >> 8) & 0XFF) * 256;
      uint32_t blue = ((buffer[(y * width) + x]) & 0XFF) * 256;
      uint32_t id = x * 6;
      byte_data[y][id] = (uint8_t)(floor((double)red / 256.0));
      byte_data[y][id + 1] = (uint8_t)(red % 256);
      byte_data[y][id + 2] = (uint8_t)(floor((double)green / 256.0));
      byte_data[y][id + 3] = (uint8_t)(green % 256);
      byte_data[y][id + 4] = (uint8_t)(floor((double)blue / 256.0));
      byte_data[y][id + 5] = (uint8_t)(blue % 256);
    }
  }
  png_init_io(png, out);
  png_set_IHDR(png, info, width, height, 16, PNG_COLOR_TYPE_RGB,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);
  png_write_info(png, info);
  png_write_image(png, byte_data);
  png_write_end(png, NULL);
  png_free_data(png, info, PNG_FREE_ALL, -1);
  png_destroy_write_struct(&png, (png_infopp)NULL);
  fclose(out);
}

std::array<double, 2> GetCenter(
    const std::array<uint64_t, 3>& tri,
    const std::vector<std::array<double, 2>>& points) {
  return {{(points[tri[0]][0] + points[tri[1]][0] + points[tri[2]][0]) / 3.0,
           (points[tri[0]][1] + points[tri[1]][1] + points[tri[2]][1]) / 3.0}};
}

void gen(int i = -1) {
  std::vector<std::array<double, 2>> points = GenPoints(variance, size);
  auto vrtedg = Delaun(points);
  std::vector<std::array<uint64_t, 3>> tri = vrtedg.first;
  std::vector<std::array<uint64_t, 3>> edg = vrtedg.second;
  for (auto& it : points) {
    it[0] *= img_width;
    it[1] *= img_height;
  }
  std::vector<uint32_t> buffer;
  double diag = std::sqrt(std::pow(img_width, 2.0) + std::pow(img_height, 2.0));
  for (unsigned y = 0; y < img_height; ++y) {
    for (unsigned x = 0; x < img_width; ++x) {
      std::array<double, 2> pt{
          {static_cast<double>(x), static_cast<double>(y)}};
      uint64_t tri_id = TriLoc(pt, points, tri, edg);
      // std::cout << pt[0] << ',' << pt[1] << "->";
      pt = GetCenter(tri[tri_id], points);
      // std::cout << pt[0] << ',' << pt[1] << '\n';
      buffer.push_back(Gradient(
          pallet,
          std::sqrt(std::pow(pt[0], 2.0) + std::pow(pt[1], 2.0)) / diag));
    }
  }
  if (i == -1) {
    WritePng(output + ".png", buffer);
  } else {
    WritePng(output + "(" + std::to_string(i) + ").png", buffer);
  }
}

int main(int argc, char* argv[]) {
  srand(time(NULL));
  ParseArgs(argc, argv);
  if (number != 1) {
    for (unsigned i = 0; i < number; ++i) {
      std::cout << "IMAGE " << i << "...\n";
      Randomize();
      gen(i);
    }
  } else {
    gen();
  }

  return 0;
}
