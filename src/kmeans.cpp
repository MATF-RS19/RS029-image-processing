#include "image.hpp"
#include <numeric>
#include <Eigen/Dense>

#define MAX_ITER 100

typedef Eigen::Matrix<unsigned long long, 3, 1> vec3ull;
// typedef cv::Vec3i vec3ull; // overflow problem

int main()
{
	img::Image<img::Type::RGB> img("images/cars.jpg");
	img::Image<img::Type::RGB> output(img.rows(), img.cols());

	int k = 10;
	std::vector<vec3ull> centroids(k);

	centroids[0] = std::accumulate(img.begin(), img.end(), vec3ull(0,0,0), [](vec3ull s, auto&& i) { 
			return vec3ull(s+vec3ull(i[0],i[1],i[2])); }) / (img.rows()*img.cols());

	for (int i = 1; i < k; i++) {
		int maxd = 0;

		for (auto&& pixel : img) {
			vec3ull p(pixel[0], pixel[1], pixel[2]);
			auto it = std::min_element(centroids.begin(), centroids.begin()+i, [&p](auto&& a, auto&& b) { return (a-p).dot(a-p) < (b-p).dot(b-p); });

			int d = (p-*it).dot(p-*it);

			if (d > maxd) {
				centroids[i] = p;
				maxd = d;
			}
		}
	}

	for (auto&& p : centroids) {
		std::cout << p.transpose() << std::endl;
	}
	std::cout << "----------------------" << std::endl;

	for (int i = 0; i < MAX_ITER; i++) {
		std::vector<vec3ull> new_centroids(k, vec3ull(0,0,0));
		std::vector<int> counter(k, 0);

		
		auto it = output.begin();

		unsigned long long f = 0;
		for (auto&& pixel : img) {
			vec3ull p(pixel[0], pixel[1], pixel[2]);
			int m = std::distance(centroids.begin(), 
								std::min_element(centroids.begin(), centroids.end(), [&p](auto&& a, auto&& b) { return (a-p).dot(a-p) < (b-p).dot(b-p); })
					);

			f += (p-centroids[m]).dot(p-centroids[m]);
			new_centroids[m] += p;
			counter[m]++;
			*it = cv::Vec3b(centroids[m][0], centroids[m][1], centroids[m][2]);
			it++;
		}

		for (int j = 0; j < k; j++) {
			if (counter[j] != 0) {
				new_centroids[j][0] = std::round(1.0*new_centroids[j][0]/counter[j]);
				new_centroids[j][1] = std::round(1.0*new_centroids[j][1]/counter[j]);
				new_centroids[j][2] = std::round(1.0*new_centroids[j][2]/counter[j]);
			}
		}

		if (std::inner_product(centroids.begin(), centroids.end(), new_centroids.begin(), 0ull, std::plus<>(), [](auto&& lhs, auto&& rhs) { return (lhs-rhs).dot(lhs-rhs); }) < 5)
			break;

		std::cout << f << std::endl;

		centroids = std::move(new_centroids);
	}

	output.show();
	cv::waitKey(0);
	output.save("kmeans_output.png");
	return 0;
}
