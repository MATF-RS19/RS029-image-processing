#include "image.hpp"
#include <numeric>

#define MAX_ITER 100

int main()
{
	img::Image<img::Type::RGB> img("images/mat.jpg");
	img::Image<img::Type::RGB> output(img.rows(), img.cols());

	int k = 7;
	std::vector<cv::Vec3i> clusters(k);

	clusters[0] = std::accumulate(img.begin(), img.end(), cv::Vec3i(0,0,0), [](cv::Vec3i s, cv::Vec3i i) { return s+i; }) / (img.rows()*img.cols());

	for (int i = 1; i < k; i++) {
		int maxd = 0;

		for (cv::Vec3i p : img) {
			auto it = std::min_element(clusters.begin(), clusters.begin()+i, [&p](auto&& a, auto&& b) { return (a-p).dot(a-p) < (b-p).dot(b-p); });

			int d = (p-*it).dot(p-*it);

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


		unsigned eps = std::inner_product(clusters.begin(), clusters.end(), new_clusters.begin(), 0, std::plus<>(), [](auto&& lhs, auto&& rhs) { return (lhs-rhs).dot(lhs-rhs); });

		std::cout << eps << std::endl; // strange convergence?!

		if (eps < 5)
			break;

		clusters = std::move(new_clusters);
	}

	output.show();
	cv::waitKey(0);
	output.save("kmeans_output.png");
	return 0;
}
