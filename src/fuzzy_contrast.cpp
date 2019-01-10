#include <cmath>
#include <algorithm>
#include "image.hpp"

int main()
{    
    /*
    img::Image<img::Type::RGB> img("images/green.png");
    int rows = img.rows();
    int columns = img.cols();
    img::Image<img::Type::RGB> output(rows, columns);

    unsigned max_R, min_R, max_G, min_G, max_B, min_B;
    max_R = min_R = img.red(0, 0);
    max_G = min_G = img.green(0, 0);
    max_B = min_B = img.blue(0, 0);

    for (int i = 1; i < rows; i++) {
        for (int j = 1; j <  columns; ++j) {
            // red
            if (img.red(i, j) > max_R) 
                max_R = img.red(i, j);
            else if (img.red(i, j) < min_R)
                min_R = img.red(i, j);
            
            // green
            if (img.green(i, j) > max_G) 
                max_G = img.green(i, j);
            else if (img.green(i, j) < min_G)
                min_G = img.green(i, j);
            
            // blue
            if (img.blue(i, j) > max_B) 
                max_B = img.blue(i, j);
            else if (img.blue(i, j) < min_B)
                min_B = img.blue(i, j);
        }
    }

    unsigned diff_R = max_R - min_R;
    unsigned diff_G = max_G - min_G;
    unsigned diff_B = max_B - min_B;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <  columns; ++j) {
            // fuzzification
            double mem_func_R = (img.red(i, j) - min_R) / double(diff_R);
            double mem_func_G = (img.green(i, j) - min_G) / double(diff_G);
            double mem_func_B = (img.blue(i, j) - min_B) / double(diff_B);

            // modification of membership function by INT operator
            
            // red channel
            if (mem_func_R <= 0.5 and mem_func_R > 0) {
                mem_func_R = 2 * pow(mem_func_R, 2);
            }
            else if (mem_func_R <= 1 and mem_func_R > 0.5) {
                mem_func_R = 1 - 2 * pow(1 - mem_func_R, 2);
            }
            // green channel
            if (mem_func_G <= 0.5 and mem_func_G > 0) {
                mem_func_G = 2 * pow(mem_func_G, 2);
            }
            else if (mem_func_G <= 1 and mem_func_G > 0.5) {
                mem_func_G = 1 - 2 * pow(1 - mem_func_G, 2);
            }
            // blue channel
            if (mem_func_B <= 0.5 and mem_func_B > 0) {
                mem_func_B = 2 * pow(mem_func_B, 2);
            }
            else if (mem_func_B <= 1 and mem_func_B > 0.5) {
                mem_func_B = 1 - 2 * pow(1 - mem_func_B, 2);
            }

            // defuzzification
            double R = min_R + mem_func_R * diff_R;
            double G = min_G + mem_func_G * diff_G;
            double B = min_B + mem_func_B * diff_B;

            cv::Vec3f defuzzified_RGB(R, G, B);
            output(i, j) = defuzzified_RGB; 
        }
    }

    */
    img::Image<img::Type::GRAYSCALE> img("images/wood1.jpeg");
    int rows = img.rows();
    int columns = img.cols();
    img::Image<img::Type::GRAYSCALE> output(rows, columns);

    // "Low Contrast Image Enhancement Technique Using Fuzzy Method"
    // Ajay Kumar Gupta, Siddharth Singh Chauhan, Manish Shrivastava
    // International Journal of Engineering Research and General Science Volume 4, Issue 2, 2016
    
    unsigned min_gray = *std::min_element(img.begin(), img.end());
    unsigned max_gray = *std::max_element(img.begin(), img.end());
    unsigned diff = max_gray - min_gray;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <  columns; ++j) {
            // fuzzification
            double mem_func = (img(i, j) - min_gray) / double(diff);
            
            // modification of membership function by INT operator
            if (mem_func <= 0.5 and mem_func > 0) {
                mem_func = 2 * pow(mem_func, 2);
            }
            else if (mem_func <= 1 and mem_func > 0.5) {
                mem_func = 1 - 2 * pow(1 - mem_func, 2);
            }

            // defuzzification
            output(i, j) = min_gray + mem_func * diff; 
        }
    }
    
    output.save("contrast.jpg");
}

    