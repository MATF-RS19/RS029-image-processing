#ifndef CANNY_HPP
#define CANNY_HPP

#include "image.hpp"

img::Image<img::Type::GRAYSCALE> canny(const img::Image<img::Type::GRAYSCALE>& img, int lower_threshold = 20, int upper_threshold = 60);

#endif // CANNY_HPP
