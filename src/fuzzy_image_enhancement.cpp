#include <cmath>
#include <algorithm>
#include "image.hpp"

#define NUM_CHANNELS 3

int main()
{
    img::Image<img::Type::RGB> img("images/wood1.jpeg");
    int rows = img.rows();
    int columns = img.cols();
    img::Image<img::Type::RGB> output(rows, columns);

    // "A Highly Efficient Color Image Contrast Enhancement using Fuzzy Based Contrast Intensification Operator"
    // Pooja Mishra, Khom Lal Sinha
    // Advanced Research in Electrical and Electronic Engineering; Volume 1, Number 4 (2014)

    unsigned max_R, min_R;
    unsigned max_G, min_G;
    unsigned max_B, min_B;
    unsigned R, G, B;
    
    max_R = min_R = img.red(0, 0);
    max_G = min_G = img.green(0, 0);
    max_B = min_B = img.blue(0, 0);
        
    for (int i = 1; i < rows; i++) {
        for (int j = 1; j <  columns; ++j) {
            R = img.red(i, j);
            G = img.green(i, j);
            B = img.blue(i, j);

            if (max_R < R) max_R = R;
            else if (min_R > R) min_R = R;
        
            if (max_G < G) max_G = G;
            else if (min_G > G) min_G = G;

            if (max_B < B) max_B = B;
            else if (min_B > B) min_B = B;
            }
    }
 
    std::vector<unsigned> max_RGB {max_R, max_G, max_B};
    std::vector<double> mid_RGB {(max_R - min_R) / 2.0, (max_G - min_G) / 2.0, (max_B - min_B) / 2.0};
    
    std::vector<unsigned> RGB;
    RGB.reserve(3);
    std::vector<unsigned> curr_RGB;
    curr_RGB.reserve(3);

    double Fe = 2.0;
    double Fd;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; ++j) {
            curr_RGB.resize(0);
            curr_RGB.push_back(img.red(i, j)); 
            curr_RGB.push_back(img.green(i, j));
            curr_RGB.push_back(img.blue(i, j));
            RGB.resize(0);

            for (int c = 0; c < NUM_CHANNELS; ++c) {
                Fd = (max_RGB[c] - mid_RGB[c]) / 0.41421356237309515;
                // fuzzification
                double mem_func = pow(1 + (max_RGB[c] - curr_RGB[c]) / Fd, -Fe);
                
                // modification of membership function by INT operator
                if (mem_func <= 0.5 and mem_func > 0) {
                    mem_func = 2 * pow(mem_func, 2);
                }
                else if (mem_func <= 1 and mem_func > 0.5) {
                    mem_func = 1 - 2 * pow(1 - mem_func, 2);
                }

                // defuzzification
                RGB.push_back(max_RGB[c] - Fd * (pow(mem_func, -1 / Fe)) + Fd);
            }
            
            cv::Vec3f BGR(RGB[2], RGB[1], RGB[0]);
            output(i, j) = BGR;
        }
    }



    /*
    unsigned max_gray = *std::max_element(img.begin(), img.end());
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <  columns; ++j) {
            double mean = (max_gray + img(i, j)) / 2;
            double var = (pow((max_gray - mean), 2) + pow((img(i, j) - mean), 2)) / 2.0;
            
            // fuzzification
            double mem_func = exp(-(pow(max_gray - img(i, j) / var, 2), 2) / 2.0);
            
            // modification of membership function by INT operator
            if (mem_func <= 0.5 and mem_func > 0) {
                mem_func = 2 * pow(mem_func, 2);
            }
            else if (mem_func <= 1 and mem_func > 0.5) {
                mem_func = 1 - 2 * pow(1 - mem_func, 2);
            }

            // defuzzification
            output(i, j) = max_gray - var * sqrt(-2.0 * log(mem_func));
        }
    }
    */

    output.save("enh.jpg");
}

    