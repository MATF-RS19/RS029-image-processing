#include "fuzzy_edge_detection.hpp"
#include <iostream>
#include <cmath>
#include <thread>
#include <mutex>

static void gaussian_blur(img::Image<img::Type::GRAYSCALE>& img)
{
	std::vector<float> gauss
		{2.0/159, 4.0/159, 5.0/159, 4.0/159, 2.0/159, 
		4.0/159, 9.0/159, 12.0/159, 9.0/159, 4.0/159,
		5.0/159, 12.0/159, 15.0/159, 12.0/159, 5.0/159,
		4.0/159, 9.0/159, 12.0/159, 9.0/159, 4.0/159,
		2.0/159, 4.0/159, 5.0/159, 4.0/159, 2.0/159};

	for (int i = 0; i < img.rows()-4; ++i) {
		for (int j = 0; j < img.cols()-4; ++j) {
			std::vector<float> current;
			current.reserve(25);
			for (int x = 0; x < 5; ++x) {
				for (int y = 0; y < 5; ++y) {
					current.push_back(img(i+x, j+y));
				}
			}

			int s = std::inner_product(gauss.cbegin(), gauss.cend(), current.cbegin(), 0.0);

			img(i+2, j+2) = s;
		}
	}
}

img::Image<img::Type::GRAYSCALE> fuzzy_edge_detection(img::Image<img::Type::GRAYSCALE> img, float threshold)
{
	gaussian_blur(img);
	auto [rows, cols] = img.dimension();

	std::vector<std::vector<double>> mi(rows, std::vector<double>(cols, 0));

	int L=256;

	double maxm=-1;

	for (int i = 1; i < rows-1; i++) {
		for (int j = 1; j < cols-1; j++) {
			double s =0;
			for (int x = -1; x <= 1; x++) {
				for (int y = -1; y <= 1; y++) {
					s+=std::abs(img(i,j)-img(i+x,j+y));
				}
			}
			mi[i][j] = s/(s+(L-1));
			maxm = std::max(maxm, mi[i][j]);
		}
	}

	for (int i = 1; i < rows-1; i++) {
		for (int j = 1; j < cols-1; j++) {
			mi[i][j] /= maxm;
			// img(i,j)=(L-1)*mi[i][j];
            img(i,j) = (mi[i][j] < threshold) ? img::BLACK : img::WHITE;
		}
	}

    return img;
}
