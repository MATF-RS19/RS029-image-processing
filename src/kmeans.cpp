#include "image.hpp"
#include <numeric>

#define MAX_ITER 100

int main()
{
	img::Image<img::Type::RGB> img("images/cars.jpg");
	img::Image<img::Type::RGB> output(img.rows(), img.cols());

	int k = 10;
	std::vector<cv::Vec3i> centroids(k);

	centroids[0] = std::accumulate(img.begin(), img.end(), cv::Vec3i(0,0,0), [](cv::Vec3i s, cv::Vec3i i) { return s+i; }) / (img.rows()*img.cols());

	for (int i = 1; i < k; i++) {
		int maxd = 0;

		for (cv::Vec3i p : img) {
			auto it = std::min_element(centroids.begin(), centroids.begin()+i, [&p](auto&& a, auto&& b) { return (a-p).dot(a-p) < (b-p).dot(b-p); });

			int d = (p-*it).dot(p-*it);

			if (d > maxd) {
				centroids[i] = p;
				maxd = d;
			}
		}
	}

	for (auto&& p : centroids) {
		std::cout << p << std::endl;
	}
	std::cout << "----------------------" << std::endl;

	for (int i = 0; i < MAX_ITER; i++) {
		std::vector<cv::Vec3i> new_centroids(k, cv::Vec3i(0,0,0));
		std::vector<int> counter(k, 0);

		
		auto it = output.begin();

		// unsigned long long f = 0;
		for (cv::Vec3i p : img) {
			int m = std::distance(centroids.begin(), 
								std::min_element(centroids.begin(), centroids.end(), [&p](auto&& a, auto&& b) { return (a-p).dot(a-p) < (b-p).dot(b-p); })
					);

			// f += (p-centroids[m]).dot(p-centroids[m]);
			new_centroids[m] += p;
			counter[m]++;
			*it++ = centroids[m];
		}

		for (int j = 0; j < k; j++) {
			if (counter[j] != 0)
				new_centroids[j] /= counter[j];
		}

		if (std::inner_product(centroids.begin(), centroids.end(), new_centroids.begin(), 0ull, std::plus<>(), [](auto&& lhs, auto&& rhs) { return (lhs-rhs).dot(lhs-rhs); }) < 5)
			break;

		// std::cout << f << std::endl;

		centroids = std::move(new_centroids);
	}

	output.show();
	cv::waitKey(0);
	output.save("kmeans_output.png");
	return 0;
}
