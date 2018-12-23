#ifndef FUZZY_EDGE_DETECTION_HPP
#define FUZZY_EDGE_DETECTION_HPP

#include "image.hpp"

img::Image<img::Type::GRAYSCALE> fuzzy_edge_detection(const img::Image<img::Type::GRAYSCALE>& img, float threshold = 0.35);

#endif // FUZZY_EDGE_DETECTION_HPP
