#ifndef DISTORTION_HPP
#define DISTORTION_HPP

#include "image.hpp"

img::Image<img::Type::RGB> distortion(const img::Image<img::Type::RGB>& image,
                                      const std::vector<std::pair<float, float>>& points_in,
                                      const std::vector<std::pair<float, float>>& points_out);

#endif // DISTORTION_HPP
