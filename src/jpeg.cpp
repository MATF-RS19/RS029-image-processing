#include <iostream>
#include <vector>
#include <eigen3/Eigen/Eigenvalues>
#include <cmath>
#include "image.hpp"

int main()
{
        img::Image<img::Type::RGB> img("images/cat.jpg");
        int rows = img.rows();
        int columns = img.cols();
        img::Image<img::Type::RGB> output(rows, columns);

        // RGB to YCbCr
        
        for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < columns; ++j) {
                        int Y = std::round(
                                16 + 0.2567890625 * img(i,j)[img::R] 
                               + 0.50412890625 * img(i,j)[img::G] 
                               + 0.09790625 * img(i,j)[img::B]
                                );
                        int Cb = std::round(
                                128 - 0.14822265625 * img(i,j)[img::R]
                                 - 0.2909921875 * img(i,j)[img::G]
                                 + 0.43921484375 * img(i,j)[img::B]
                                );  
                        int Cr = std::round(
                                128 + 0.43921484375 * img(i,j)[img::R]
                                 - 0.3677890625 * img(i,j)[img::G]
                                 - 0.07142578125 * img(i,j)[img::B]
                            );
                        // (Y, Y, Y) ---> Grayscale
                        cv::Vec3f yCrCbPixel(Cr, Cb, Y);
                        output(i,j) = yCrCbPixel;
                        
                }
        }

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

        output.save("ycbcr.jpeg");
}