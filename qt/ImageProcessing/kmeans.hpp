#ifndef KMEANS_HPP
#define KMEANS_HPP 

#include "image.hpp"

img::Image<img::Type::RGB> color_quantization(const img::Image<img::Type::RGB>& img, int k);

#endif /* KMEANS_HPP */
