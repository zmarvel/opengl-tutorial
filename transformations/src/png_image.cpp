
#include "png_image.hpp"

PngImage::PngImage(std::string path)
  : image{path}
{
}

PngImage::~PngImage() {
}

void PngImage::readRaw(std::unique_ptr<unsigned char[]>& dest) {
  uint32_t height = image.get_height();
  uint32_t width = image.get_width();
  for (uint32_t row = 0; row < height; row++) {
    // Read rows in reverse or the image will be displayed upside-down
    png::image<png::rgba_pixel>::row_access rowData = image[height-row-1];
    for (size_t i = 0; i < width; i++) {
      png::rgba_pixel pix = rowData[i];

      size_t rowOffs = row*sizeof(png::rgba_pixel)*width;
      dest[rowOffs+4*i+0] = pix.red;
      dest[rowOffs+4*i+1] = pix.green;
      dest[rowOffs+4*i+2] = pix.blue;
      dest[rowOffs+4*i+3] = pix.alpha;
    }
  }
}

std::unique_ptr<unsigned char[]> PngImage::readRaw() {
  uint32_t height = image.get_height();
  uint32_t width = image.get_width();
  uint32_t imageSize = width * height;
  std::unique_ptr<unsigned char[]> rawImage{
    new unsigned char[imageSize*sizeof(png::rgba_pixel)]};
  readRaw(rawImage);
  return rawImage;
}
