#include "image.hpp"
#include <iostream>
#include <cmath>
#include <mutex>


// fuzzy c-means clustering
// we have two clusters (black and white)
// our aim is to partition pixels into these clusters
class fcm {
public:
	fcm(const img::Image<img::Type::GRAYSCALE>& img)
		: m_img(img),
		  m_output(img.rows(), img.cols()),
		  m_w1(img.rows(), std::vector<int>(img.cols(), 0))
	{
		fuzzification();
	}

	const img::Image<img::Type::GRAYSCALE>& binarize()
	{
		fuzzy_cmeans();
		img::start_threads(0, m_output.rows(), &fcm::defuzzification, this);
		return m_output;
	}

private:
	const img::Image<img::Type::GRAYSCALE>& m_img;
	img::Image<img::Type::GRAYSCALE> m_output;
	// degree to which pixel (i,j) belongs to cluster 1
	std::vector<std::vector<int>> m_w1;
	// the centroid of a cluster 1
	unsigned long long m_centroid_1;
	// the centroid of a cluster 2
	unsigned long long m_centroid_2;
	std::mutex m_mutex; 

	void fuzzification_help(int from, int to)
	{
		for (int i = from; i < to; i++) {
			std::transform(m_img[i], m_img[i]+m_img.cols(), m_w1[i].begin(), [](auto&& p) { return 10*p/255; });
		}
	}

	void fuzzification()
	{
		img::start_threads(0, m_img.rows(), &fcm::fuzzification_help, this);
	}

	void centroids_help(unsigned& denominator_1, unsigned& denominator_2, int from, int to)
	{
		unsigned denominator_1_tmp = 0;
		unsigned denominator_2_tmp = 0;
		unsigned long long centroid_1_tmp = 0;
		unsigned long long centroid_2_tmp = 0;


		for (int i = from; i < to; i++) {
			denominator_1_tmp += std::inner_product(m_w1[i].begin(), m_w1[i].end(), m_w1[i].begin(), 0u);
			denominator_2_tmp += std::inner_product(m_w1[i].begin(), m_w1[i].end(), m_w1[i].begin(), 0u, std::plus<>(), [](auto&& lhs, auto&& rhs) { return (10-lhs)*(10-rhs); });
			centroid_1_tmp += std::inner_product(m_w1[i].begin(), m_w1[i].end(), m_img[i], 0ull, std::plus<>(), [](auto&& lhs, auto&& rhs) { return lhs*lhs*rhs; });
			centroid_2_tmp += std::inner_product(m_w1[i].begin(), m_w1[i].end(), m_img[i], 0ull, std::plus<>(), [](auto&& lhs, auto&& rhs) { return (10-lhs)*(10-lhs)*rhs; });
		}

		centroid_1_tmp /= 100;
		centroid_2_tmp /= 100;
		denominator_1_tmp /= 100;
		denominator_2_tmp /= 100;

		std::lock_guard<std::mutex> lock(m_mutex);
		m_centroid_1 += centroid_1_tmp;
		m_centroid_2 += centroid_2_tmp;
		denominator_1 += denominator_1_tmp;
		denominator_2 += denominator_2_tmp;
	}

	void centroids()
	{
		m_centroid_1 = 0;
		m_centroid_2 = 0;

		unsigned denominator_1 = 0;
		unsigned denominator_2 = 0;

		img::start_threads(0, m_img.rows(), &fcm::centroids_help, this, denominator_1, denominator_2);

		m_centroid_1 /= denominator_1;
		m_centroid_2 /= denominator_2;
	}


	void update_weights_help(unsigned& eps, int from, int to)
	{
		unsigned eps_tmp = 0;

		for (int i = from; i < to; i++) {
			for (int j = 0; j < m_img.cols(); j++) {
				unsigned d1 = (m_centroid_1-m_img(i,j))*(m_centroid_1-m_img(i,j));
				unsigned d2 = (m_centroid_2-m_img(i,j))*(m_centroid_2-m_img(i,j));
				unsigned a = std::round(10.0*d2/(d1+d2));

				eps_tmp += (m_w1[i][j]-a)*(m_w1[i][j]-a);

				m_w1[i][j] = a;
			}
		}

		eps_tmp /= 100;

		std::lock_guard<std::mutex> lock(m_mutex);
		eps += eps_tmp;
	}

	unsigned update_weights()
	{
		unsigned eps = 0;
		img::start_threads(0, m_img.rows(), &fcm::update_weights_help, this, eps);
		return eps;
	}

	void fuzzy_cmeans()
	{
		int max_iter = 0;

		do {
			centroids();
		} while(update_weights() > 1 && ++max_iter < 30);
	}

	void defuzzification(int from, int to)
	{
		for (int i = from; i < to; i++) {
			std::transform(m_w1[i].begin(), m_w1[i].end(), m_output[i], [](auto&& p) { return (p > 10-p) ? img::WHITE : img::BLACK; });
		}
	}
};


img::Image<img::Type::GRAYSCALE> binarization(const img::Image<img::Type::GRAYSCALE>& img)
{
	return fcm(img).binarize();
}


int main()
{

	// img::Image<img::Type::GRAYSCALE> img("images/blackboard.jpg");
	// if (!img) return -1;
	// auto tstart = std::time(0);

	// for (int i = 0; i < 50; i++) {
	// 	// auto output = canny(img);
	// 	auto binary = binarization(img);
	// }
	// std::cout << std::time(0)-tstart << std::endl;

	img::Image<img::Type::GRAYSCALE> img("images/blackboard.jpg");
	// img::Image<img::Type::GRAYSCALE> img("images/storm_trooper.jpg");

	auto binary = binarization(img);

	binary.show();
	cv::waitKey(0);
	binary.save("binary_output.png");

    return 0;
}
