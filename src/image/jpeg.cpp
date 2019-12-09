#include "jpeg.hpp"

#include <cstdlib>
#include <cstring>

#include <array>
#include <string>
#include <vector>

#include <jpeglib.h>

#include "../config.hpp"
#include "../terminal.hpp"

std::pair<std::array<unsigned, 2>, std::vector<COLOR>> read_jpeg(
    const std::string& file) {}
bool write_jpeg(const std::string& file, const unsigned& w, const unsigned& h,
                const std::vector<COLOR>& pixels) {
  FILE* out = fopen(file.c_str(), "w");
  if (!out) {
    error("Failed to open file \"" + file + "\"");
    return false;
  }
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);
  jpeg_stdio_dest(&cinfo, out);
  cinfo.image_width = w;
  cinfo.image_height = h;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;
  jpeg_set_defaults(&cinfo);
  jpeg_start_compress(&cinfo, true);
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
  jpeg_write_scanlines(&cinfo, byte_data, cinfo.image_height);
  jpeg_finish_compress(&cinfo);
  for (std::size_t i = 0; i < cinfo.image_height; ++i) {
    free(byte_data[i]);
  }
  free(byte_data);
  jpeg_destroy_compress(&cinfo);
  fclose(out);
  return true;
}
