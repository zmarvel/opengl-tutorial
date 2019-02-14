
#ifndef PNG_IMAGE_HPP
#define PNG_IMAGE_HPP

#include <memory>
#include <string>

#include <png++/png.hpp>

class PngImage {
  public:
    explicit PngImage(std::string path);
    ~PngImage();

    void readRaw(std::unique_ptr<unsigned char[]>& dest);
    std::unique_ptr<unsigned char[]> readRaw();
    std::array<uint32_t, 2> getDims() const {
      return std::array<uint32_t, 2>{image.get_height(), image.get_width()};
    }
    uint32_t getWidth() const {
      return image.get_width();
    }
    uint32_t getHeight() const {
      return image.get_height();
    }

  private:
    png::image<png::rgba_pixel> image;
};


#endif // PNG_IMAGE_HPP
