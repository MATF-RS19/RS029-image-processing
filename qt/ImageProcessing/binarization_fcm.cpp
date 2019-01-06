#include "binarization.hpp"
#include <vector>

// fuzzy c-means clustering
// we have two clusters (black and white)
// our aim is to partition pixels into these clusters
class fcm {
public:
	fcm(const img::Image<img::Type::GRAYSCALE>& img)
		: m_img(img),
		m_output(img.rows(), img.cols()),
		m_w1(img.rows(), std::vector<float>(img.cols(), 0))
	{
		fuzzification();
	}

	const img::Image<img::Type::GRAYSCALE>& binarize()
	{
		fuzzy_cmeans();
		defuzzification();
		return m_output;
	}

private:
	// input image
	const img::Image<img::Type::GRAYSCALE>& m_img;
	// binarized image
	img::Image<img::Type::GRAYSCALE> m_output;
	// m_w1[i][j] describes how much pixel (i,j) belongs to the first cluster (white)
	// 1-m_w1[i][j] describes how much pixel (i,j) belongs to the second cluster (black)
	std::vector<std::vector<float>> m_w1;
	// centroids of white and black cluster
	std::pair<float, float> m_centroids;

	void fuzzification()
	{
		for (int i = 0; i < m_img.rows(); i++) {
			for (int j = 0; j < m_img.cols(); j++) {
				m_w1[i][j] = m_img(i,j)/255.0;
			}
		}
	}

	// get centroids based on weights and return the distance from the previous centroids
	float centroids()
	{
		std::pair<float, float> c1_fraction = {0,0};
		std::pair<float, float> c2_fraction = {0,0};

		for (int i = 0; i < m_img.rows(); i++) {
			for (int j = 0; j < m_img.cols(); j++) {
				c1_fraction.first += m_w1[i][j]*m_w1[i][j]*m_img(i,j);
				c1_fraction.second += m_w1[i][j]*m_w1[i][j];
				c2_fraction.first += (1-m_w1[i][j])*(1-m_w1[i][j])*m_img(i,j);
				c2_fraction.second += (1-m_w1[i][j])*(1-m_w1[i][j]);
			}
		}

		auto centroids = m_centroids;
		m_centroids = {c1_fraction.first/c1_fraction.second, c2_fraction.first/c2_fraction.second};

		return (centroids.first-m_centroids.first)*(centroids.first-m_centroids.first)+(centroids.second-m_centroids.second)*(centroids.second-m_centroids.second);
	}


	void update_weights()
	{
		for (int i = 0; i < m_img.rows(); i++) {
			for (int j = 0; j < m_img.cols(); j++) {
				float d1 = (m_img(i,j)-m_centroids.first)*(m_img(i,j)-m_centroids.first);
				float d2 = (m_img(i,j)-m_centroids.second)*(m_img(i,j)-m_centroids.second);
				m_w1[i][j] = d2/(d2+d1);
			}
		}
	}

	void fuzzy_cmeans()
	{
		int max_iter = 0;
		float eps = 0;

		do {
			eps = centroids();
			update_weights();
		} while(eps > 1 && ++max_iter < 30);
	}

	// cluster each pixel of the output image
	void defuzzification()
	{
		for (int i = 0; i < m_img.rows(); i++) {
			for (int j = 0; j < m_img.cols(); j++) {
				m_output(i,j) = (m_w1[i][j] > 0.5) ? 255 : 0;
			}
		}
	}
};

img::Image<img::Type::GRAYSCALE> binarization_fcm(const img::Image<img::Type::GRAYSCALE>& img)
{
	return fcm(img).binarize();
}

