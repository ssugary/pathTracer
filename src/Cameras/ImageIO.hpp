#pragma once

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "Utils/common.hpp"
#include <vector>

namespace ImageIO
 {
  bool write(const std::string&, int, int, const std::vector<Color>&, bool=false);

 } // namespace ImageIO 

#endif // IMAGE_HPP
