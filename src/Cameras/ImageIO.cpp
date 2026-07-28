
#include "ImageIO.hpp"
#include "lodepng/lodepng.h"
#include <algorithm>
#include <fstream>
#include <iostream>


namespace ImageIO 
{

  inline float apply_gamma(float value, float gamma=2.2) 
  {
    if (value <= 0.0) return 0.0;
    if (value >= 1.0) return 1.0;
    return std::pow(value, 1.0 / gamma);
  }


  bool writePPM(const std::string& filename, int w, int h, const std::vector<Color>& buffer, bool gamma)
  {
    std::ofstream ofs(filename);

    if (!ofs.is_open()) {
      std::cerr << "Error on create the file " + filename << '\n';
      return false;
    }
    ofs << "P3\n" << w << ' ' << h << "\n255\n";

    for (auto &color : buffer) {
        Color c = ::clamp(color, 0.f, 1.f);

      if(gamma){
        c.r = apply_gamma(c.r);
        c.g = apply_gamma(c.g);
        c.b = apply_gamma(c.b);
      }

      int ir = static_cast<int>(c.r * 255.f);
      int ig = static_cast<int>(c.g * 255.f);
      int ib = static_cast<int>(c.b * 255.f);

      ofs << ir << ' ' << ig << ' ' << ib << '\n';
    }

    ofs.close();
    return true; // STUB

  }

  bool writePNG(const std::string& filename, int w, int h, const std::vector<Color>& buffer, bool gamma)
  {
    std::vector<float> img;
    img.reserve(w * h * 4);

    for (auto &color : buffer) {
        Color c = ::clamp(color, 0.f, 1.f);

      if(gamma)
      {
       c.r = apply_gamma(c.r);
       c.g = apply_gamma(c.g);
       c.b = apply_gamma(c.b);
      }

      img.push_back(static_cast<float>(c.r * 255.f));
      img.push_back(static_cast<float>(c.g * 255.f));
      img.push_back(static_cast<float>(c.b * 255.f));
      img.push_back(255); // blk = 255.
    }

     std::vector<unsigned char> chars(img.begin(), img.end());

    unsigned error = lodepng::encode(filename, chars, w, h);
    if (error) {
      std::cout << "encoder error " << error << ": " << lodepng_error_text(error)
                << "\n";
      return false;
    } else {
      std::cout << "image generated in: " << filename << "\n";
   }
    return true; // STUB

  }


  bool write(const std::string& filename, int w, int h, const std::vector<Color>& buffer, bool gamma)
  {
    if(filename.find(".ppm") != std::string::npos)
    {
      return writePPM(filename, w, h, buffer, gamma);
    }
    else
    {
      return writePNG(filename, w, h, buffer, gamma);
    }

  }
}


