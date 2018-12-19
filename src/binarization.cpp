#include "image.hpp"
#include <iostream>
#include <cmath>
#include <mutex>


class fcm {
public:
	fcm(const img::Image<img::Type::GRAYSCALE>& img)
		: m_img(img),
		  m_output(img.rows(), img.cols()),
		  m1(img.rows(), std::vector<float>(img.cols(), 0)),
		  m2(img.rows(), std::vector<float>(img.cols(), 0))
	{
		init_m();
	}

	const img::Image<img::Type::GRAYSCALE>& binarize()
	{
		fuzzy_cluster_means();
		img::start_threads(0, m_output.rows(), &fcm::set_output, this);
		return m_output;
	}

private:
	const img::Image<img::Type::GRAYSCALE>& m_img;
	img::Image<img::Type::GRAYSCALE> m_output;
	std::vector<std::vector<float>> m1;
	std::vector<std::vector<float>> m2;
	double C1;
	double C2;
	std::mutex mu; 

	void init_m_help(int from, int to)
	{
		for (int i = from; i < to; i++) {
			for (int j = 0; j < m_img.cols(); j++) {
				m1[i][j] = m_img(i,j)/255.0;
				m2[i][j] = 1 - m1[i][j];
			}
		}
	}

	void init_m()
	{
		img::start_threads(0, m_img.rows(), &fcm::init_m_help, this);
	}

	void centroids_help(double& m1_sum, double& m2_sum, int from, int to)
	{
		double m1_sum_tmp = 0;
		double m2_sum_tmp = 0;
		double C1_tmp = 0;
		double C2_tmp = 0;

		for (int i = from; i < to; i++) {
			for (int j = 0; j < m_img.cols(); j++) {
				double x = m1[i][j]*m1[i][j];
				C1_tmp += x*m_img(i,j);
				m1_sum_tmp += x;

				x = m2[i][j]*m2[i][j];
				C2_tmp += x*m_img(i,j);
				m2_sum_tmp += x;
			}
		}

		std::lock_guard<std::mutex> lock(mu);
		C1 += C1_tmp;
		C2 += C2_tmp;
		m1_sum += m1_sum_tmp;
		m2_sum += m2_sum_tmp;
	}

	void centroids()
	{
		C1 = 0;
		C2 = 0;
		double m1_sum = 0;
		double m2_sum = 0;

		img::start_threads(0, m_img.rows(), &fcm::centroids_help, this, m1_sum, m2_sum);

		C1 /= m1_sum;
		C2 /= m2_sum;
	}


	void update_m_help(double& sum_u, int from, int to)
	{
		double sum_u_tmp = 0;

		for (int i = from; i < to; i++) {
			for (int j = 0; j < m_img.cols(); j++) {
				float d1=(C1-m_img(i,j))*(C1-m_img(i,j));
				float d2=(C2-m_img(i,j))*(C2-m_img(i,j));
				float a = d2/(d1+d2);
				float b = d1/(d1+d2);
				sum_u_tmp += (m1[i][j]-a)*(m1[i][j]-a) + (m2[i][j]-b)*(m2[i][j]-b);
				m1[i][j] = a;
				m2[i][j] = b;
			}
		}

		std::lock_guard<std::mutex> lock(mu);
		sum_u += sum_u_tmp;
	}

	double update_m()
	{
		double sum_u = 0;
		img::start_threads(0, m_img.rows(), &fcm::update_m_help, this, sum_u);
		return sum_u;
	}

	void fuzzy_cluster_means()
	{
		int max_iter = 0;

		do {
			centroids();
		} while(update_m() > 1 && ++max_iter < 30);
	}

	void set_output(int from, int to)
	{
		for (int i = from; i < to; i++) {
			for (int j = 0; j < m_img.cols(); j++) {
				m_output(i,j) = (m1[i][j] > m2[i][j]) ? img::WHITE : img::BLACK;
			}
		}
	}
};


img::Image<img::Type::GRAYSCALE> binarization(const img::Image<img::Type::GRAYSCALE> img)
{
	return fcm(img).binarize();
}


int main()
{

	// auto tstart = std::time(0);
	// img::Image<img::Type::GRAYSCALE> img("images/blackboard.jpg");
	// if (!img) return -1;

	// for (int i = 0; i < 50; i++) {
	// 	// auto output = canny(img);
	// 	auto binary = binarization(img);
	// }
	// std::cout << std::time(0)-tstart << std::endl;

	img::Image<img::Type::GRAYSCALE> img("images/blackboard.jpg");

	auto binary = binarization(img);

	binary.show();
	cv::waitKey(0);
	binary.save("binary_output.png");

    return 0;
}
