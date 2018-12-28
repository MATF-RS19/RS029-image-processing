#include "image.hpp"
#include <numeric>

#define RAND (std::rand()%256)
#define RANDOM_PIXEL (cv::Vec3i(RAND, RAND, RAND))
#define MAX_ITER 100

int main()
{
	std::srand(std::time(nullptr));
	img::Image<img::Type::RGB> img("images/cars.jpg");
	img::Image<img::Type::RGB> output(img.rows(), img.cols());

	int k = 10;
	std::vector<cv::Vec3i> clusters(k);

	cv::Vec3i avg;
	for (cv::Vec3i p : img) {
		avg += p;
	}
	avg /= (img.rows()*img.cols());
	clusters[0] = avg;
	for (int i = 1; i < k; i++) {
		int maxd = 0;

		for (cv::Vec3i p : img) {
			int d = 999999999;
			for (int j = 0; j < i; j++) {
				if ((p-clusters[j]).dot(p-clusters[j]) < d) {
					d = (p-clusters[j]).dot(p-clusters[j]);
				}
			}

			if (d > maxd) {
				clusters[i] = p;
				maxd = d;
			}
		}
	}

	for (auto&& p : clusters) {
		std::cout << p << std::endl;
	}
	std::cout << "----------------------" << std::endl;
	// return 0;

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
			if (counter[j] != 0)
				new_clusters[j] /= counter[j];
		}


		// unsigned eps = std::inner_product(img.begin(), img.end(), output.begin(), 0, std::plus<>(), [](auto&& lhs, auto&& rhs) { return (lhs-rhs).dot(lhs-rhs); });
		unsigned eps = std::inner_product(clusters.begin(), clusters.end(), new_clusters.begin(), 0, std::plus<>(), [](auto&& lhs, auto&& rhs) { return (lhs-rhs).dot(lhs-rhs); });

		if (eps < 1)
			break;

		clusters = std::move(new_clusters);
		std::cout << eps << std::endl; // strange convergence?!
	}

	output.show();
	cv::waitKey(0);
	output.save("kmeans_output.png");
	return 0;
}
