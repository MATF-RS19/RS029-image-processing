#include "image.hpp"
#include <numeric>
#include <mutex>
#include <Eigen/Dense>

#define MAX_ITER 100

typedef Eigen::Matrix<unsigned long long, 3, 1> vec3;
// typedef cv::Vec3i vec3; // overflow problem

static std::mutex mu;

void avg_help(const img::Image<img::Type::RGB>& img, vec3& sum, int from, int to)
{
	vec3 sum_tmp = std::accumulate(img[from], img[to], vec3(0,0,0),
			[](vec3 s, auto&& i) { return vec3(s+vec3(i[0],i[1],i[2])); });

	std::lock_guard<std::mutex> lock(mu);
	sum += sum_tmp;
}

vec3 avg(const img::Image<img::Type::RGB>& img)
{
	vec3 sum(0,0,0);
	img::start_threads(0, img.rows(), avg_help, img, sum);
	return sum/img.pixels();
}

void set_centroid_help(const img::Image<img::Type::RGB>& img, std::vector<vec3>& centroids, int i, int& maxd, int from, int to)
{
	int maxd_tmp = 0;
	vec3 centroid_tmp;

	std::for_each(img[from], img[to], [&](auto&& pixel) {
		vec3 p(pixel[0], pixel[1], pixel[2]);
		auto it = std::min_element(centroids.begin(), centroids.begin()+i, [&p](auto&& a, auto&& b) { return (a-p).dot(a-p) < (b-p).dot(b-p); });

		int d = (p-*it).dot(p-*it);

		if (d > maxd_tmp) {
			centroid_tmp = p;
			maxd_tmp = d;
		}
	});

	std::lock_guard<std::mutex> lock(mu);
	if (maxd_tmp > maxd) {
		maxd = maxd_tmp;
		centroids[i] = centroid_tmp;
	}
}

void set_centroid(const img::Image<img::Type::RGB>& img, std::vector<vec3>& centroids, int i)
{
	int maxd = 0;
	img::start_threads(0, img.rows(), set_centroid_help, img, centroids, i, maxd);
}

void update_help(const img::Image<img::Type::RGB>& img,img::Image<img::Type::RGB>& output, const std::vector<vec3>& centroids, unsigned long long& f, std::vector<vec3>& new_centroids, std::vector<int>& counter, int from, int to)
{
	unsigned long long f_tmp = 0;
	int k = centroids.size();
	std::vector<vec3> new_centroids_tmp(k, vec3(0,0,0));
	std::vector<int> counter_tmp(k, 0);

	for (int x = from; x<to; ++x) {
		for (int y = 0; y < img.cols(); ++y) {
			vec3 p(img[x][y][0], img[x][y][1], img[x][y][2]);
			int m = std::distance(centroids.begin(), 
								std::min_element(centroids.begin(), centroids.end(), [&p](auto&& a, auto&& b) { return (a-p).dot(a-p) < (b-p).dot(b-p); })
					);

			output[x][y] = cv::Vec3b(centroids[m][0], centroids[m][1], centroids[m][2]);

			f_tmp += (p-centroids[m]).dot(p-centroids[m]);
			new_centroids_tmp[m] += p;
			counter_tmp[m]++;
		}
	}

	std::lock_guard<std::mutex> lock(mu);
	f += f_tmp;
	for (int i = 0; i < k; i++) {
		counter[i] += counter_tmp[i];
		new_centroids[i] += new_centroids_tmp[i];
	}
}

unsigned long long update(const img::Image<img::Type::RGB>& img,img::Image<img::Type::RGB>& output, std::vector<vec3>& centroids)
{
	unsigned long long f = 0;
	int k = centroids.size();
	std::vector<vec3> new_centroids(k, vec3(0,0,0));
	std::vector<int> counter(k, 0);
	
	img::start_threads(0, img.rows(), update_help, img, output, centroids, f, new_centroids, counter);

	for (int j = 0; j < k; j++) {
		if (counter[j] != 0) {
			new_centroids[j][0] = std::round(1.0*new_centroids[j][0]/counter[j]);
			new_centroids[j][1] = std::round(1.0*new_centroids[j][1]/counter[j]);
			new_centroids[j][2] = std::round(1.0*new_centroids[j][2]/counter[j]);
		}
	}

	unsigned long long eps = std::inner_product(centroids.begin(), centroids.end(), new_centroids.begin(), 0ull, std::plus<>(), [](auto&& lhs, auto&& rhs) { return (lhs-rhs).dot(lhs-rhs); });

	std::cout << f << std::endl;

	centroids = std::move(new_centroids);

	return eps;
}

int main()
{
	img::Image<img::Type::RGB> img("images/cars.jpg");
	img::Image<img::Type::RGB> output(img.rows(), img.cols());

	int k = 10;
	std::vector<vec3> centroids(k);

	centroids[0] = avg(img);

	for (int i = 1; i < k; i++) {
		set_centroid(img, centroids, i);
	}

	for (auto&& p : centroids) {
		std::cout << p.transpose() << std::endl;
	}
	std::cout << "----------------------" << std::endl;

	for (int i = 0; i < MAX_ITER; i++) {
		auto eps = update(img, output, centroids);
		if (eps < 5)
			break;
	}

	output.show();
	cv::waitKey(0);
	output.save("kmeans_output.png");
	return 0;
}


// 118 119 125
// 255 255 255
// 18 14 19
//   5  37 190
// 131 184 251
// 212 193 158
// 105  71 231
//  32 153 233
//  70  32 104
// 176 121 206
// ----------------------
// 3905495268
// 1397731840
// 1197466304
// 1087476780
// 1021711575
// 984011887
// 958787080
// 938774437
// 920426986
// 904346855
// 889389410
// 879487472
// 873339401
// 868847301
// 865885757
// 863703941
// 861834822
// 859764438
// 858492163
// 857324776
// 856480589
