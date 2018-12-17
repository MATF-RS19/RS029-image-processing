#include "image.hpp"
#include <iostream>
#include <cmath>
#include <mutex>

static float m = 2;
static std::mutex mu; 

void set_picture(const std::vector<std::vector<float>>& m1, const std::vector<std::vector<float>>& m2,
		img::Image<img::Type::GRAYSCALE>& img, int from, int to)
{
	for (int i = from; i < to; i++) {
		for (int j = 0; j < img.cols(); j++) {
			img(i,j) = (m1[i][j]>m2[i][j]) ? img::WHITE : img::BLACK;
		}
	}
}

void init_m(std::vector<std::vector<float>>& m1, std::vector<std::vector<float>>& m2,
		const img::Image<img::Type::GRAYSCALE>& img, int from, int to)
{
	for (int i = from; i < to; i++) {
		for (int j = 0; j < img.cols(); j++) {
			m1[i][j] = img(i,j)/255.0;
			m2[i][j] = 1 - m1[i][j];
		}
	}
}


void centroids(double& C1, double& C2, double& m1_sum, double& m2_sum,
		const std::vector<std::vector<float>>& m1, const std::vector<std::vector<float>>& m2,
		const img::Image<img::Type::GRAYSCALE>& img, int from, int to)
{
	double m1_sum_tmp = 0;
	double m2_sum_tmp = 0;
	double C1_tmp = 0;
	double C2_tmp = 0;

	for (int i = from; i < to; i++) {
		for (int j = 0; j < img.cols(); j++) {
			double x = std::pow(m1[i][j], m);
			C1_tmp += x*img(i,j);
			m1_sum_tmp += x;

			x = std::pow(m2[i][j], m);
			C2_tmp += x*img(i,j);
			m2_sum_tmp += x;
		}
	}

	std::lock_guard<std::mutex> lock(mu);
	C1 += C1_tmp;
	C2 += C2_tmp;
	m1_sum += m1_sum_tmp;
	m2_sum += m2_sum_tmp;
}

void update_m(double& sum_u, 
		std::vector<std::vector<float>>& m1, std::vector<std::vector<float>>& m2,
		double C1, double C2, const img::Image<img::Type::GRAYSCALE>& img, int from, int to)
{
	double sum_u_tmp = 0;

	for (int i = from; i < to; i++) {
		for (int j = 0; j < img.cols(); j++) {
			float d1=((C1-img(i,j))*(C1-img(i,j)));
			float d2=((C2-img(i,j))*(C2-img(i,j)));
			float d_sq1 = 1.0/d1;
			float d_sq2 = 1.0/d2;
			float a = std::pow(d_sq1, 1.0/(m-1));
			float b = std::pow(d_sq2, 1.0/(m-1));
			sum_u_tmp += std::pow(m1[i][j]-a/(a+b),2) + std::pow(m2[i][j]-b/(a+b),2);

			m1[i][j] = a/(a+b);
			m2[i][j] = b/(a+b);
		}
	}

	std::lock_guard<std::mutex> lock(mu);
	sum_u += sum_u_tmp;
}

int main()
{
	// img::Image<img::Type::GRAYSCALE> img("images/r2d2.jpg");
	img::Image<img::Type::GRAYSCALE> img("images/blackboard.jpg");
	// img::Image<img::Type::GRAYSCALE> img("images/storm_trooper.jpg");
	// img::Image<img::Type::GRAYSCALE> img("images/mat.jpg");

	auto&& [rows, cols] = img.dimension();

	img::Image<img::Type::GRAYSCALE> binary(rows, cols);

	std::vector<std::vector<float>> m1(rows, std::vector<float>(cols, 0));
	std::vector<std::vector<float>> m2(rows, std::vector<float>(cols, 0));

	img::start_threads(0, rows, init_m, m1, m2, img);

	double sum_u = 0;
	int iter=0;


	do {
		++iter;

		double C1 = 0;
		double C2 = 0;
		double m1_sum = 0;
		double m2_sum = 0;

		img::start_threads(0, rows, centroids, C1, C2, m1_sum, m2_sum, m1, m2, img);

		C1 /= m1_sum;
		C2 /= m2_sum;

		sum_u = 0;

		img::start_threads(0, rows, update_m, sum_u, m1, m2, C1, C2, img);

		if (iter > 30) break;
		// std::cout << C1 << std::endl;
	} while(sum_u>1);

	std::cout << "iterations: " << iter << std::endl;

	img::start_threads(0, rows, set_picture, m1, m2, binary);

	binary.show();
	cv::waitKey(0);
	binary.save("binary_output.png");

    return 0;
}
