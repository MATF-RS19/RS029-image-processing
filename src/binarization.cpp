#include "image.hpp"
#include <iostream>
#include <cmath>

int main()
{
	// img::Image<img::Type::GRAYSCALE> img("images/r2d2.jpg");
	// img::Image<img::Type::GRAYSCALE> img("images/blackboard.jpg");
	img::Image<img::Type::GRAYSCALE> img("images/storm_trooper.jpg");
	// img::Image<img::Type::GRAYSCALE> img("images/text.png");
	auto [rows, cols] = img.dimension();

	img::Image<img::Type::GRAYSCALE> binary(rows, cols);

	std::vector<std::vector<float>> m1(rows, std::vector<float>(cols, 0));
	std::vector<std::vector<float>> m2(rows, std::vector<float>(cols, 0));

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			m1[i][j] = img(i,j)/255.0;
			m2[i][j] = 1 - m1[i][j];
		}
	}


	double m = 2;

	float sum_u = 0;
	int iter=0;

	do {
		iter++;
		float C1 = 0;
		float m1_sum = 0;

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				float x = std::pow(m1[i][j], m);
				C1 += x*img(i,j);
				m1_sum += x;
			}
		}
		C1 /= m1_sum;

		float C2=0;
		float m2_sum = 0;

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				float x = std::pow(m2[i][j], m);
				C2 += x*img(i,j);
				m2_sum += x;
			}
		}
		C2 /= m2_sum;

		sum_u = 0;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				float d1=(C1-img(i,j))*(C1-img(i,j));
				float d2=(C2-img(i,j))*(C2-img(i,j));
				float d_sq1 = 1.0/d1;
				float d_sq2 = 1.0/d2;
				float a = std::pow(d_sq1, 1.0/(m-1));
				float b = std::pow(d_sq2, 1.0/(m-1));
				sum_u += std::pow(m1[i][j]-a/(a+b),2) + std::pow(m2[i][j]-b/(a+b),2);

				m1[i][j] = a/(a+b);
				m2[i][j] = b/(a+b);
			}
		}

	} while(sum_u>1);

	std::cout << "iterations: " << iter << std::endl;


	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			binary(i,j) = (m1[i][j]>m2[i][j]) ? img::WHITE : img::BLACK;
			// if (m1[i][j]>0.9) {
			// 	binary(i,j)=img::WHITE;
			// }
			// else {
			// 	float median;
			// 	if (i==0 || j==0 || i==rows-1 || j==cols-j)
			// 		median = m1[i][j];
			// 	else {
			// 		std::vector<float> v;
			// 		for (int a = -1; a <= 1; a++) {
			// 			for (int b = -1; b <= 1; b++) {
			// 				v.push_back(m1[i+a][j+b]);
			// 			}
			// 		}
			// 		std::sort(v.begin(), v.end());
			// 		median = v[4];
			// 	}

			// 	if (median > m2[i][j])
			// 		binary(i,j)=img::WHITE;
			// 	else
			// 		binary(i,j)=img::BLACK;
			// }
		}
	}

	binary.show();
	cv::waitKey(0);
	binary.save("binary_output.png");

    return 0;
}
