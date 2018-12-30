#ifndef BINARIZATION_H
#define BINARIZATION_H

#include "image.hpp"

img::Image<img::Type::GRAYSCALE> binarization_fcm(const img::Image<img::Type::GRAYSCALE>& img);
img::Image<img::Type::GRAYSCALE> binarization_kmeans(const img::Image<img::Type::GRAYSCALE>& img);

#endif // BINARIZATION_H
