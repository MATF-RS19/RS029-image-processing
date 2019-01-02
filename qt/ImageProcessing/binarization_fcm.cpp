#include "binarization.hpp"
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
		  m_output(img.rows(), img.cols())
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
	std::pair<unsigned, unsigned> m_centroids;
	std::mutex m_mutex; 

	void fuzzification_help(int from, int to)
	{
		for (int i = from; i < to; i++) {
			std::transform(m_img[i], m_img[i]+m_img.cols(), m_output[i], [](auto&& p) { return 10*p/255; });
		}
	}

	void fuzzification()
	{
		img::start_threads(0, m_img.rows(), &fcm::fuzzification_help, this);
	}

	void centroids_help(std::pair<unsigned long long, unsigned long long>& c1_fraction, std::pair<unsigned long long, unsigned long long>& c2_fraction, int from, int to)
	{
		std::pair<unsigned long long, unsigned long long> c1_fraction_tmp;
		std::pair<unsigned long long, unsigned long long> c2_fraction_tmp;

		for (int i = from; i < to; i++) {
			c1_fraction_tmp.second += std::inner_product(m_output[i], m_output[i+1], m_output[i], 0u);
			c2_fraction_tmp.second += std::inner_product(m_output[i], m_output[i+1], m_output[i], 0u, std::plus<>(), [](auto&& lhs, auto&& rhs) { return (10-lhs)*(10-rhs); });
			c1_fraction_tmp.first += std::inner_product(m_output[i], m_output[i+1], m_img[i], 0ull, std::plus<>(), [](auto&& lhs, auto&& rhs) { return lhs*lhs*rhs; });
			c2_fraction_tmp.first += std::inner_product(m_output[i], m_output[i+1], m_img[i], 0ull, std::plus<>(), [](auto&& lhs, auto&& rhs) { return (10-lhs)*(10-lhs)*rhs; });
		}

		c1_fraction_tmp.first /= 100;
		c1_fraction_tmp.second /= 100;
		c2_fraction_tmp.first /= 100;
		c2_fraction_tmp.second /= 100;

		std::lock_guard<std::mutex> lock(m_mutex);
		c1_fraction.first += c1_fraction_tmp.first;
		c1_fraction.second += c1_fraction_tmp.second;
		c2_fraction.first += c2_fraction_tmp.first;
		c2_fraction.second += c2_fraction_tmp.second;
	}

	unsigned centroids()
	{
		std::pair<unsigned long long, unsigned long long> c1_fraction = {0,0};
		std::pair<unsigned long long, unsigned long long> c2_fraction = {0,0};
		img::start_threads(0, m_img.rows(), &fcm::centroids_help, this, c1_fraction, c2_fraction);

		std::pair<unsigned, unsigned> centroids = m_centroids;
		m_centroids = {c1_fraction.first/c1_fraction.second, c2_fraction.first/c2_fraction.second};

		return (centroids.first-m_centroids.first)*(centroids.first-m_centroids.first)+(centroids.second-m_centroids.second)*(centroids.second-m_centroids.second);
	}


	void update_weights_help(int from, int to)
	{
		for (int i = from; i < to; i++) {
			for (int j = 0; j < m_img.cols(); j++) {
				unsigned d1 = (m_centroids.first-m_img(i,j))*(m_centroids.first-m_img(i,j));
				unsigned d2 = (m_centroids.second-m_img(i,j))*(m_centroids.second-m_img(i,j));
				m_output(i,j) = std::round(10.0*d2/(d1+d2));
			}
		}
	}

	void update_weights()
	{
		img::start_threads(0, m_img.rows(), &fcm::update_weights_help, this);
	}

	void fuzzy_cmeans()
	{
		int max_iter = 0;
		unsigned eps = 0;

		do {
			eps = centroids();
			update_weights();
		} while(eps > 1 && ++max_iter < 30);
	}

	void defuzzification(int from, int to)
	{
		for (int i = from; i < to; i++) {
			std::transform(m_output[i], m_output[i+1], m_output[i], [](auto&& p) { return (p > 10-p) ? img::WHITE : img::BLACK; });
		}
	}
};


img::Image<img::Type::GRAYSCALE> binarization_fcm(const img::Image<img::Type::GRAYSCALE>& img)
{
	return fcm(img).binarize();
}

