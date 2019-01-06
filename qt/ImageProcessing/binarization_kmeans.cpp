#include "binarization.hpp"
#include <numeric>
#include <mutex>
#include <Eigen/Dense>

// k-means algorithm for image binarization
class kmeans {
public:
	kmeans(const img::Image<img::Type::GRAYSCALE>& img, int maxiter = 100)
		: m_img(img), m_output(m_img.rows(), m_img.cols()), m_maxiter(maxiter)
	{}

	const img::Image<img::Type::GRAYSCALE>& cluster()
	{
		set_first_centroid();
		set_second_centroid();

		if (m_centroids.first > m_centroids.second)
			std::swap(m_centroids.first,m_centroids.second);

		for (int i = 0; i < m_maxiter; i++) {
			if (update() < 5)
				break;
		}

		return m_output;
	}

private:
	// input image
	const img::Image<img::Type::GRAYSCALE>& m_img;
	// resulting image
	img::Image<img::Type::GRAYSCALE> m_output;
	// centroids of clusters
	std::pair<int, int> m_centroids;
	// maximum number of iteration
	int m_maxiter;
	std::mutex m_mutex;

	// the first centroid is an average of pixels
	void set_first_centroid_help(unsigned long long& sum, int from, int to)
	{
		unsigned long long sum_tmp = std::accumulate(m_img[from], m_img[to], 0ull);

		std::lock_guard<std::mutex> lock(m_mutex);
		sum += sum_tmp;
	}

	void set_first_centroid()
	{
		unsigned long long sum = 0;
		img::start_threads(0, m_img.rows(), &kmeans::set_first_centroid_help, this, sum);
		m_centroids.first = sum/m_img.pixels();
	}

	// the second centroid is the pixel that is farthest from the first centroid
	void set_second_centroid_help(int& maxd, int from, int to)
	{
		int maxd_tmp = 0;
		int centroid_tmp;

		std::for_each(m_img[from], m_img[to], [&](auto&& p) {
					int d = std::abs(p-m_centroids.first);

					if (d > maxd_tmp) {
						centroid_tmp = p;
						maxd_tmp = d;
					}
				});

		std::lock_guard<std::mutex> lock(m_mutex);
		if (maxd_tmp > maxd) {
			maxd = maxd_tmp;
			m_centroids.second = centroid_tmp;
		}
	}

	void set_second_centroid()
	{
		int maxd = 0;
		img::start_threads(0, m_img.rows(), &kmeans::set_second_centroid_help, this, maxd);
	}

	// update output and get new centroids
	void update_help(std::pair<unsigned long long, unsigned long long>& new_centroids, std::pair<int, int>& counter, int from, int to)
	{
		std::pair<unsigned long long, unsigned long long> new_centroids_tmp{0,0};
		std::pair<int,int> counter_tmp{0,0};

		for (int x = from; x < to; ++x) {
			for (int y = 0; y < m_img.cols(); ++y) {
				int p = m_img(x,y);

				if (std::abs(m_centroids.first-p) <= std::abs(m_centroids.second-p)) {
					m_output[x][y] = img::BLACK;

					new_centroids_tmp.first += p;
					counter_tmp.first++;
				}
				else {
					m_output[x][y] = img::WHITE;

					new_centroids_tmp.second += p;
					counter_tmp.second++;
				}
			}
		}

		std::lock_guard<std::mutex> lock(m_mutex);
		counter.first += counter_tmp.first;
		counter.second += counter_tmp.second;
		new_centroids.first += new_centroids_tmp.first;
		new_centroids.second += new_centroids_tmp.second;
	}

	unsigned long long update()
	{
		std::pair<unsigned long long, unsigned long long> new_centroids{0,0};
		std::pair<int, int> counter{0,0};

		img::start_threads(0, m_img.rows(), &kmeans::update_help, this, new_centroids, counter);

		if (counter.first)
			new_centroids.first = std::round(1.0*new_centroids.first/counter.first);
		if (counter.second)
			new_centroids.second = std::round(1.0*new_centroids.second/counter.second);

		// difference between centroids
		unsigned long long eps =(new_centroids.first-m_centroids.first)*(new_centroids.first-m_centroids.first)+(new_centroids.second-m_centroids.second)*(new_centroids.second-m_centroids.second);

		// update centroids
		m_centroids = std::move(new_centroids);

		return eps;
	}
};

img::Image<img::Type::GRAYSCALE> binarization_kmeans(const img::Image<img::Type::GRAYSCALE>& img)
{
	return kmeans(img).cluster();
}
