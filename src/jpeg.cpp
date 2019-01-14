#include <iostream>
#include <vector>
#include <eigen3/Eigen/Eigenvalues>
#include <cmath>
#include <algorithm>
#include <iterator>
#include "image.hpp"


int main()
{
        
        img::Image<img::Type::GRAYSCALE> img("images/woman.jpeg");
        int rows = img.rows();
        int columns = img.cols();
        img::Image<img::Type::GRAYSCALE> output(rows, columns);
        
        output = img.fuzzy_grayscale_contrast_basic();
        output.save("hsi.jpg");

        //output = img.rgb2ycrcb();
        /*
        // DCT
        Eigen::MatrixXf M(8,8);
        M.row(0) << (0.7071067811865476, 0.7071067811865476, 0.7071067811865476, 0.70710678118654760.7071067811865476, 0.70710678118654760.7071067811865476, 0.7071067811865476);
        for (int i = 1; i < 8; ++i) {
                for (int j = 1; j < 8; ++j) {
                        M(i,j) = cos((M_PI*j) / 16)
                }
        }
        for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < columns; ++j) {
                        output(i,j) = img(i,j) - 127;
                }
        }
        */

      // output.ycrcb().save("jpeg.jpeg");
        
        return 0;
}