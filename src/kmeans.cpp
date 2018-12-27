#include "image.hpp"
#include <numeric>

#define RAND (std::rand()%256)
#define RANDOM_PIXEL (cv::Vec3i(RAND, RAND, RAND))
#define MAX_ITER 100

int main()
{
	img::Image<img::Type::RGB> img("images/cars.jpg");
	img::Image<img::Type::RGB> output(img.rows(), img.cols());

	int k = 5;
	std::vector<cv::Vec3i> clusters(k);
	std::fill(clusters.begin(), clusters.end(), RANDOM_PIXEL);


	for (int i = 0; i < MAX_ITER; i++) {
		std::vector<cv::Vec3i> new_clusters(k, cv::Vec3i(0,0,0));
		std::vector<int> counter(k, 0);

		
		auto it = output.begin();

		for (cv::Vec3i p : img) {
				int m = std::distance(clusters.begin(), 
									std::min_element(clusters.begin(), clusters.end(), [&p](auto&& a, auto&& b) { return (a-p).dot(a-p) < (b-p).dot(b-p); })
						);

				new_clusters[m] += p;
				counter[m]++;
				*it++ = clusters[m];
		}

		for (int j = 0; j < k; j++) {
			new_clusters[j] /= counter[j];
		}

		if (std::inner_product(clusters.begin(), clusters.end(), new_clusters.begin(), 0, std::plus<>(), 
			[](auto&& lhs, auto&& rhs) { return (lhs-rhs).dot(lhs-rhs); }) < 1)
			break;

		clusters = std::move(new_clusters);
	}

	output.show();
	cv::waitKey(0);
	output.save("kmeans_output.png");
	return 0;
}
