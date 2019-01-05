#include <iostream>
#include <vector>
#include <eigen3/Eigen/Eigenvalues>
#include <cmath>
#include <algorithm>
#include <iterator>
#include "image.hpp"

#define RGB_SCALE 255

int main()
{
        
        img::Image<img::Type::RGB> img("2.jpeg");
        int rows = img.rows();
        int columns = img.cols();
        img::Image<img::Type::RGB> output(rows, columns);

        /*
        // RGB to HSI
        for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < columns; ++j) {
                        std::vector<unsigned> RGB {img.red(i, j), img.green(i, j), img.blue(i, j)};
                        unsigned min_RGB = std::distance(RGB.cbegin(), std::min_element(RGB.cbegin(), RGB.cend()));
                        unsigned R_B = img.red(i, j) - img.blue(i, j);
                        unsigned R_G = img.red(i, j) - img.green(i, j);
                        unsigned H, S, I;
                        
                        unsigned sum_pixel = img.pixel_sum(i, j);
                        
                        H = 1 / ((0.5 * R_G + R_B) / sqrt(R_G * R_G + R_B * R_G));
                        S = 1 - 3 * min_RGB / sum_pixel;
                        I = sum_pixel / 3;
                        
                        cv::Vec3f HSIPixel(H, S, I);
                        output(i,j) = HSIPixel;
                }
        }
        
        
        // RGB to HSB
        for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < columns; ++j) {
                        std::vector<unsigned> RGB {img.red(i, j), img.green(i, j), img.blue(i, j)};
                        unsigned max_RGB = std::distance(RGB.cbegin(), std::max_element(RGB.cbegin(), RGB.cend()));
                        unsigned min_RGB = std::distance(RGB.cbegin(), std::min_element(RGB.cbegin(), RGB.cend()));
                        
                        int delta = RGB[max_RGB] - RGB[min_RGB];
                        double H, S, B;

                        if (delta < 0) {
                                H = 0;
                                S = 0;
                                B = 0;
                        }
                        else {
                                double R1 = (RGB[max_RGB] - img.red()) / delta;
                                double G1 = (RGB[max_RGB] - img.green()) / delta;
                                double B1 = (RGB[max_RGB] - img.blue()) / delta;

                                if (max_RGB == 0) {
                                        H = B1 - G1;  
                                }
                                else if (max_RGB == 1) {
                                        H = R1 - B1 + 2;
                                }
                                else {
                                        H = G1 - R1 + 4;
                                }

                                if (H < 0) {
                                        H = H / 6 + 1;
                                }

                                S = RGB[max_RGB] / delta;
                                B = RGB[max_RGB] / RGB_SCALE;
                                
                                cv::Vec3f HSBPixel(H, S, B);
                                output(i,j) = HSBPixel;
                                
                        }
                }
        }
        
        //  YCrCb to RGB
        // formula source: https://www.w3.org/Graphics/JPEG/jfif3.pdf

        for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < columns; ++j) {
                        int R = std::round(
                                img.red(i, j) 
                                + 1.402 * (img.green(i, j) - 128) 
                                );
                        int G = std::round(
                                img.red(i, j)
                                - 0.34414 * (img.blue(i, j) - 128)
                                - 0.71414 * (img.green(i, j) - 128)
                                );  
                        int B = std::round(
                                img.red(i, j) + 1.772 * (img.blue(i, j) - 128)
                                );

                        cv::Vec3f RGB(B, G, R);
                        output(i,j) = RGB;
                        
                }
        }

        */

        // RGB to YCbCr
        // formula source: https://www.w3.org/Graphics/JPEG/jfif3.pdf

        for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < columns; ++j) {
                        int Y = std::round(
                                0.299 * img.red(i, j) 
                                + 0.587 * img.green(i, j) 
                                + 0.114 * img.blue(i, j)
                                );
                        int Cb = std::round(
                                128 - 0.1687 * img.red(i, j)
                                - 0.3313 * img.green(i, j)
                                + 0.5 * img.blue(i, j)
                                );  
                        int Cr = std::round(
                                128 + 0.5 * img.red(i, j)
                                - 0.4187 * img.green(i, j)
                                - 0.0813 * img.blue(i, j)
                            );

                        cv::Vec3f yCrCbPixel(Y, Cr, Cb);
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

        output.save("jpeg.jpeg");
        
        return 0;
}