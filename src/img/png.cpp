#include "png.hpp"

#include <cstring>
#include <string>

#include <png.h>

#include "image.hpp"

bool image::write_png(const std::string& file_path, const Image& img) {
  png_structp png =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) {
    return false;
  }
  png_infop info = png_create_info_struct(png);
  if (!info) {
    return false;
  }
  if (setjmp(png_jmpbuf(png))) {
    return false;
  }
  FILE* out = fopen(file_path.c_str(), "w");
  if (!out) {
    return false;
  }
  uint8_t** byte_data = (uint8_t**)malloc(img.height_ * sizeof(uint8_t*));
  if (!byte_data) {
    fclose(out);
    return false;
  }
  for (uint32_t i = 0; i < img.height_; ++i) {
    byte_data[i] = (uint8_t*)std::malloc(3 * img.width_ * sizeof(uint8_t));
    if (!byte_data[i]) {
      for (uint32_t j = 0; j < i; ++j) {
        free(byte_data[j]);
      }
      free(byte_data);
      fclose(out);
      return false;
    }
    std::memset(byte_data[i], 0x00, 3 * img.width_ * sizeof(uint8_t));
    for (uint32_t j = 0; j < img.width_; ++j) {
      // uint32_t id = 6 * j;
      uint32_t id = 3 * j;
      uint16_t red = (img(j, i) >> 16) & 0xFF;
      uint16_t blue = (img(j, i) >> 8) & 0xFF;
      uint16_t green = (img(j, i) >> 0) & 0xFF;
      // byte_data[i][id] = (red >> 8) & 0xff;
      // byte_data[i][id + 1] = (red & 0xff);
      // byte_data[i][id + 2] = (green >> 8) & 0xff;
      // byte_data[i][id + 3] = (green & 0xff);
      // byte_data[i][id + 4] = (blue >> 8) & 0xff;
      // byte_data[i][id + 5] = (blue & 0xff);

      byte_data[i][id + 0] = (red & 0xff);
      byte_data[i][id + 1] = (green & 0xff);
      byte_data[i][id + 2] = (blue & 0xff);
    }
  }
  png_init_io(png, out);
  png_set_IHDR(png, info, img.width_, img.height_, 8, PNG_COLOR_TYPE_RGB,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
               PNG_FILTER_TYPE_DEFAULT);
  png_write_info(png, info);
  png_write_image(png, byte_data);
  png_write_end(png, NULL);
  png_free_data(png, info, PNG_FREE_ALL, -1);
  png_destroy_write_struct(&png, (png_infopp)NULL);
  for (uint32_t i = 0; i < img.height_; ++i) {
    free(byte_data[i]);
  }
  free(byte_data);
  fclose(out);
  return true;
}
