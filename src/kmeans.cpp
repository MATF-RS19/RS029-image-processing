#include "image.hpp"
#include <numeric>
#include <mutex>
#include <Eigen/Dense>

typedef Eigen::Matrix<unsigned long long, 3, 1> vec3;
// typedef cv::Vec3i vec3; // overflow problem

class kmeans {
public:
	kmeans(const img::Image<img::Type::RGB>& img, int k, int maxiter = 100)
		: m_img(img), m_output(m_img.rows(), m_img.cols()), m_centroids(std::vector<vec3>(k)),
		m_maxiter(maxiter), m_f(0)
	{}

	const img::Image<img::Type::RGB>& cluster()
	{
		int k = m_centroids.size();

		m_centroids[0] = avg();

		for (int i = 1; i < k; i++) {
			set_centroid(i);
		}

		// for (auto&& p : m_centroids) {
		// 	std::cout << p.transpose() << std::endl;
		// }
		// std::cout << "----------------------" << std::endl;

		for (int i = 0; i < m_maxiter; i++) {
			if (update() < 5)
				break;
		}

		return m_output;
	}

private:
	std::mutex m_mutex;
	// input image
	const img::Image<img::Type::RGB>& m_img;
	// resulting image
	img::Image<img::Type::RGB> m_output;
	// centroids of clusters
	std::vector<vec3> m_centroids;
	// maximum number of iteration
	int m_maxiter;
	// objective function value
	unsigned long long m_f;

	void avg_help(vec3& sum, int from, int to)
	{
		vec3 sum_tmp = std::accumulate(m_img[from], m_img[to], vec3(0,0,0),
				[](vec3 s, auto&& i) { return vec3(s+vec3(i[0],i[1],i[2])); });

		std::lock_guard<std::mutex> lock(m_mutex);
		sum += sum_tmp;
	}

	vec3 avg()
	{
		vec3 sum(0,0,0);
		img::start_threads(0, m_img.rows(), &kmeans::avg_help, this, sum);
		return sum/m_img.pixels();
	}

	void set_centroid_help(int i, int& maxd, int from, int to)
	{
		int maxd_tmp = 0;
		vec3 centroid_tmp;

		std::for_each(m_img[from], m_img[to], [&](auto&& pixel) {
			vec3 p(pixel[0], pixel[1], pixel[2]);
			auto it = std::min_element(m_centroids.begin(), m_centroids.begin()+i, [&p](auto&& a, auto&& b) { return (a-p).dot(a-p) < (b-p).dot(b-p); });

			int d = (p-*it).dot(p-*it);

			if (d > maxd_tmp) {
				centroid_tmp = p;
				maxd_tmp = d;
			}
		});

		std::lock_guard<std::mutex> lock(m_mutex);
		if (maxd_tmp > maxd) {
			maxd = maxd_tmp;
			m_centroids[i] = centroid_tmp;
		}
	}

	void set_centroid(int i)
	{
		int maxd = 0;
		img::start_threads(0, m_img.rows(), &kmeans::set_centroid_help, this, i, maxd);
	}

	// update output, get new centroids and objective function value
	void update_help(std::vector<vec3>& new_centroids, std::vector<int>& counter, int from, int to)
	{
		int k = m_centroids.size();
		unsigned long long f_tmp = 0;
		std::vector<vec3> new_centroids_tmp(k, vec3(0,0,0));
		std::vector<int> counter_tmp(k, 0);

		for (int x = from; x < to; ++x) {
			for (int y = 0; y < m_img.cols(); ++y) {
				vec3 p(m_img[x][y][0], m_img[x][y][1], m_img[x][y][2]);

				int m = std::distance(m_centroids.begin(), 
									std::min_element(m_centroids.begin(), m_centroids.end(), 
									[&p](auto&& a, auto&& b) { return (a-p).dot(a-p) < (b-p).dot(b-p); })
						);

				m_output[x][y] = cv::Vec3b(m_centroids[m][0], m_centroids[m][1], m_centroids[m][2]);

				f_tmp += (p-m_centroids[m]).dot(p-m_centroids[m]);
				new_centroids_tmp[m] += p;
				counter_tmp[m]++;
			}
		}

		std::lock_guard<std::mutex> lock(m_mutex);
		m_f += f_tmp;
		for (int i = 0; i < k; i++) {
			counter[i] += counter_tmp[i];
			new_centroids[i] += new_centroids_tmp[i];
		}
	}

	unsigned long long update()
	{
		int k = m_centroids.size();
		std::vector<vec3> new_centroids(k, vec3(0,0,0));
		std::vector<int> counter(k, 0);
		// reset value of objective function
		m_f = 0;
		
		img::start_threads(0, m_img.rows(), &kmeans::update_help, this, new_centroids, counter);

		for (int j = 0; j < k; j++) {
			if (counter[j] != 0) {
				new_centroids[j][0] = std::round(1.0*new_centroids[j][0]/counter[j]);
				new_centroids[j][1] = std::round(1.0*new_centroids[j][1]/counter[j]);
				new_centroids[j][2] = std::round(1.0*new_centroids[j][2]/counter[j]);
			}
		}

		// difference between centroids
		unsigned long long eps = std::inner_product(m_centroids.begin(), m_centroids.end(), new_centroids.begin(), 0ull, std::plus<>(), [](auto&& lhs, auto&& rhs) { return (lhs-rhs).dot(lhs-rhs); });

		// this function should decrease
		std::cout << m_f << std::endl;

		// update centroids
		m_centroids = std::move(new_centroids);

		return eps;
	}
};

// color color_quantization of img using k colors
img::Image<img::Type::RGB> color_quantization(const img::Image<img::Type::RGB>& img, int k)
{
	return kmeans(img, k).cluster();
}

int main()
{
	img::Image<img::Type::RGB> img("images/blackboard.jpg");

	auto tstart = std::time(0);
	// for (int i = 0; i < 10; i++) {
		auto output = color_quantization(img, 2);
	// }
	std::cout << std::time(0)-tstart << std::endl;

	output.show();
	cv::waitKey(0);
	output.save("kmeans_output.png");
	return 0;
}
