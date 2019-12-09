#include "png.hpp"

#include <cstdlib>
#include <cstring>

#include <array>
#include <string>
#include <vector>

#include <png.h>

#include "../config.hpp"
#include "../terminal.hpp"

std::pair<std::array<unsigned, 2>, std::vector<COLOR>> read_png(
    const std::string& file) {
  FILE* load = std::fopen(file.c_str(), "rb");
  std::pair<std::array<unsigned, 2>, std::vector<COLOR>> res;
  if (!load) {
    error("Failed to open file \"" + file + "\"");
  }
  uint8_t header[8];
  fread(header, 1, 8, load);
  if (png_sig_cmp(header, 0, 8)) {
    fclose(load);
    error("Failed to open file \"" + file + "\"");
    return res;
  }
  png_structp png =
      png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) {
    error("Failed to create PNG write struct");
    return res;
  }
  png_infop info = png_create_info_struct(png);
  if (!info) {
    error("Failed to create PNG info struct");
    return res;
  }
  if (setjmp(png_jmpbuf(png))) {
    return res;
  }
  png_init_io(png, load);
  png_set_sig_bytes(png, 8);
  png_read_info(png, info);
  unsigned w = png_get_image_width(png, info);
  unsigned h = png_get_image_height(png, info);
  png_byte color_type = png_get_color_type(png, info);
  png_byte bit_depth = png_get_bit_depth(png, info);

  if (bit_depth == 16) {
    png_set_strip_16(png);
  }
  if (color_type == PNG_COLOR_TYPE_PALETTE) {
    png_set_palette_to_rgb(png);
  } else if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
    png_set_expand_gray_1_2_4_to_8(png);
  }

  if (png_get_valid(png, info, PNG_INFO_tRNS)) {
    png_set_tRNS_to_alpha(png);
  }

  if (color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
    png_set_gray_to_rgb(png);
  }

  png_read_update_info(png, info);

  uint8_t** byte_data = (uint8_t**)std::malloc(sizeof(uint8_t*) * h);
  for (unsigned y = 0; y < h; ++y) {
    byte_data[y] = (uint8_t*)std::malloc(png_get_rowbytes(png, info));
  }
  png_read_image(png, byte_data);

  res.first = {w, h};
  res.second = std::vector<COLOR>(w * h, 0x0);

  for (unsigned i = 0; i < h; ++i) {
    for (unsigned j = 0; j < w; ++j) {
      uint32_t id = j * 3;
#if COLOR_DEPTH == 16
      if (bit_depth == 16) {
        res.second[i * w + j] =
            (byte_data[i][id + 0] << 40) | (byte_data[i][id + 1] << 32) |
            (byte_data[i][id + 2] << 24) | (byte_data[i][id + 3] << 16) |
            (byte_data[i][id + 4] << 8) | (byte_data[i][id + 5] << 0);
      } else {
        res.second[i * w + j] =
            ((static_cast<uint64_t>(byte_data[i][id + 0]) * 257) << 32) |
            ((static_cast<uint64_t>(byte_data[i][id + 1]) * 257) << 16) |
            ((static_cast<uint64_t>(byte_data[i][id + 2]) * 257) << 0);
      }
#else
      if (bit_depth == 16) {
        res.second[i * w + j] =
            (static_cast<uint8_t>(
                 static_cast<double>((byte_data[i][id + 0] << 8) |
                                     (byte_data[i][id + 1] << 0)) /
                 257.0)
             << 16) |
            (static_cast<uint8_t>(
                 static_cast<double>((byte_data[i][id + 2] << 8) |
                                     (byte_data[i][id + 3] << 0)) /
                 257.0)
             << 8) |
            (static_cast<uint8_t>(
                 static_cast<double>((byte_data[i][id + 4] << 8) |
                                     (byte_data[i][id + 5] << 0)) /
                 257.0)
             << 0);
      } else {
        res.second[i * w + j] =
            (static_cast<uint32_t>(byte_data[i][id + 0]) << 16) |
            (static_cast<uint32_t>(byte_data[i][id + 1]) << 8) |
            (static_cast<uint32_t>(byte_data[i][id + 2]) << 0);
      }
#endif
    }
  }

  for (uint32_t i = 0; i < h; ++i) {
    free(byte_data[i]);
  }
  free(byte_data);
  fclose(load);
  png_destroy_read_struct(&png, &info, NULL);
  return res;
}
bool write_png(const std::string& file, const unsigned& w, const unsigned& h,
               const std::vector<COLOR>& pixels) {
  png_structp png =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) {
    error("Failed to create PNG write struct");
    return false;
  }
  png_infop info = png_create_info_struct(png);
  if (!info) {
    error("Failed to create PNG info struct");
    return false;
  }
  if (setjmp(png_jmpbuf(png))) {
    return false;
  }
  FILE* out = fopen(file.c_str(), "w");
  if (!out) {
    error("Failed to open file \"" + file + "\"");
    return false;
  }
  uint8_t** byte_data = (uint8_t**)std::malloc(h * sizeof(uint8_t*));
  if (!byte_data) {
    fclose(out);
    return false;
  }
  for (uint32_t i = 0; i < h; ++i) {
#if COLOR_DEPTH == 16
    byte_data[i] = (uint8_t*)std::malloc(6 * w * sizeof(uint8_t));
#else
    byte_data[i] = (uint8_t*)std::malloc(3 * w * sizeof(uint8_t));
#endif
    if (!byte_data[i]) {
      for (uint32_t j = 0; j < i; ++j) {
        free(byte_data[j]);
      }
      free(byte_data);
      fclose(out);
      return false;
    }
#if COLOR_DEPTH == 16
    std::memset(byte_data[i], 0x00, 6 * w * sizeof(uint8_t));
#else
    std::memset(byte_data[i], 0x00, 3 * w * sizeof(uint8_t));
#endif
    for (uint32_t j = 0; j < w; ++j) {
#if COLOR_DEPTH == 16
      uint32_t id = 6 * j;
      byte_data[i][id + 0] = (pixels[i * w + j] >> 40) & 0xff;
      byte_data[i][id + 1] = (pixels[i * w + j] >> 32) & 0xff;
      byte_data[i][id + 2] = (pixels[i * w + j] >> 24) & 0xff;
      byte_data[i][id + 3] = (pixels[i * w + j] >> 16) & 0xff;
      byte_data[i][id + 4] = (pixels[i * w + j] >> 8) & 0xff;
      byte_data[i][id + 5] = (pixels[i * w + j] >> 0) & 0xff;
#else
      uint32_t id = 3 * j;
      byte_data[i][id + 0] = (pixels[i * w + j] >> 16) & 0xff;
      byte_data[i][id + 1] = (pixels[i * w + j] >> 8) & 0xff;
      byte_data[i][id + 2] = (pixels[i * w + j] >> 0) & 0xff;
#endif
    }
  }
  png_init_io(png, out);
  png_set_IHDR(png, info, w, h, COLOR_DEPTH, PNG_COLOR_TYPE_RGB,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);
  png_write_info(png, info);
  png_write_image(png, byte_data);
  png_write_end(png, NULL);
  png_free_data(png, info, PNG_FREE_ALL, -1);
  png_destroy_write_struct(&png, (png_infopp)NULL);
  for (uint32_t i = 0; i < h; ++i) {
    free(byte_data[i]);
  }
  free(byte_data);
  fclose(out);
  return true;
}
