#include <iostream>
#include <vector>
#include "image.hpp"

#define MAX_BINS 256

int main()  
{
    // For GRAYSCALE image
    img::Image<img::Type::GRAYSCALE> img("images/cat.jpg");
    int rows = img.rows();
    int columns = img.cols();
    img::Image<img::Type::RGB> output(rows, columns);

    std::vector<unsigned> histogram (MAX_BINS, 0);
    double mean = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <  columns; ++j) {
            histogram[img(i, j)]++;
            mean += img(i, j);
        }
    }

    mean /= rows * columns;

    /*
    FOR RGB 

    std::vector<unsigned> R_histogram(MAX_BINS, 0);
    std::vector<unsigned> G_histogram(MAX_BINS, 0);
    std::vector<unsigned> B_histogram(MAX_BINS, 0);
    
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            R_histogram[img.red(i, j)]++;
            G_histogram[img.green(i, j)]++;
            B_histogram[img.blue(i, j)]++;
        }
    }

    std::copy(R_histogram.cbegin(), R_histogram.cend(), std::ostream_iterator<unsigned>(std::cout, " "));
    std::cout << std::endl;
    */

    return 0;
}
