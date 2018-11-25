#include "binarization.h"
//#include "image.hpp"
#include <vector>
#include <iostream>
#include <cmath>
#include <thread>
#include <mutex>

static float m = 2;
static std::mutex mu; 

void set_picture(const std::vector<std::vector<float>>& m1, const std::vector<std::vector<float>>& m2,
		img::Image<img::Type::GRAYSCALE>& img, int from, int to)
{
	for (int i = from; i < to; i++) {
		for (int j = 0; j < img.cols(); j++) {
			img(i,j) = (m1[i][j]>m2[i][j]) ? img::WHITE : img::BLACK;
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

img::Image<img::Type::GRAYSCALE> binarization(const img::Image<img::Type::GRAYSCALE>& img)
{
	auto [rows, cols] = img.dimension();

	img::Image<img::Type::GRAYSCALE> binary(rows, cols);

	std::vector<std::vector<float>> m1(rows, std::vector<float>(cols, 0));
	std::vector<std::vector<float>> m2(rows, std::vector<float>(cols, 0));

	int num_threads = 4;
	std::vector<std::thread> threads(num_threads);
	for (int i = 0; i < num_threads; i++) {
		int from = rows/num_threads * i;
		int to = (i==num_threads-1) ? rows : from + rows/num_threads;
		threads[i] = std::thread(init_m, std::ref(m1), std::ref(m2), std::ref(img), from, to);
	}

	for (int i = 0; i < num_threads; i++) {
		threads[i].join();
	}


	double sum_u = 0;
	int iter=0;


	do {
		iter++;

		double C1 = 0;
		double C2 = 0;
		double m1_sum = 0;
		double m2_sum = 0;

		for (int i = 0; i < num_threads; i++) {
			int from = rows/num_threads * i;
			int to = (i==num_threads-1) ? rows : from + rows/num_threads;
			threads[i] = std::thread(centroids, std::ref(C1), std::ref(C2),
									std::ref(m1_sum), std::ref(m2_sum),
									std::ref(m1), std::ref(m2), std::ref(img), from, to);
		}

		for (int i = 0; i < num_threads; i++) {
			threads[i].join();
		}

		C1 /= m1_sum;
		C2 /= m2_sum;

		sum_u = 0;

		for (int i = 0; i < num_threads; i++) {
			int from = rows/num_threads * i;
			int to = (i==num_threads-1) ? rows : from + rows/num_threads;
			threads[i] = std::thread(update_m,  std::ref(sum_u),
									std::ref(m1), std::ref(m2),
									C1, C2,
									 std::ref(img), from, to);
		}

		for (int i = 0; i < num_threads; i++) {
			threads[i].join();
		}

		if (iter > 30) break;
		// std::cout << C1 << std::endl;
	} while(sum_u>1);

	std::cout << "iterations: " << iter << std::endl;

	for (int i = 0; i < num_threads; i++) {
		int from = rows/num_threads * i;
		int to = (i==num_threads-1) ? rows : from + rows/num_threads;
		threads[i] = std::thread(set_picture,  
								std::ref(m1), std::ref(m2),
								 std::ref(binary), from, to);
	}

	for (int i = 0; i < num_threads; i++) {
		threads[i].join();
	}

    return binary;
}