#include <cmath>
#include <algorithm>
#include "image.hpp"

int main()
{
    img::Image<img::Type::GRAYSCALE> img("images/storm_trooper.jpg");
    int rows = img.rows();
    int columns = img.cols();
    img::Image<img::Type::GRAYSCALE> output(rows, columns);

    //unsigned min_gray = *std::min_element(img.begin(), img.end());
    unsigned max_gray = *std::max_element(img.begin(), img.end());
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <  columns; ++j) {
            double mean = (max_gray + img(i, j)) / 2;
            double var = (pow((max_gray - mean), 2) + pow((img(i, j) - mean), 2)) / 2;
            
            double mem_func = exp(-(pow(max_gray - img(i, j) / var, 2), 2) / 2);
            
            if (mem_func <= 0.5) {
                mem_func = 2 * pow(mem_func, 2);
            }
            else if (mem_func <= 1) {
                mem_func = 1 - 2 * pow(1 - mem_func, 2);
            }

            output(i, j) = max_gray - var * sqrt(-2.0 * log(mem_func));
        }
    }

    output.save("enh.jpg");
}

    