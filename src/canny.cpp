#include "image.hpp"
#include <vector>
#include <numeric>
#include <stack>
#include <thread>

static const int ALMOST_WHITE = 254;

static const double pi = 3.14159265358979323846;

// Check two pixels that belong to the parallel lines below and above the line of a pixel (i,j). If magnitude of pixel (i,j) is greater than the magnitude of those two pixels return true.
static bool is_maximum(int i, int j, const std::vector<std::vector<float>>&  magnitude, const std::vector<std::vector<float>>& angles)
{
	int angle = angles[i][j];
	std::pair<int, int> n1, n2;
	if (angle == 0) {
		// vertical line
		n1 = {i, j-1};
		n2 = {i, j+1};
	}
	else if (angle == 45) {
		// line is from top right to bottom left (careful here! angles go in the negative direction)
		n1 = {i-1, j-1};
		n2 = {i+1, j+1};
	}
	else if (angle == 90) {
		// horizontal line
		n1 = {i-1, j};
		n2 = {i+1, j};
	}
	else if (angle == 135) {
		// line is from top left to bottom right (careful here! angles go in the negative direction)
		n1 = {i-1, j+1};
		n2 = {i+1, j-1};
	}

	return (magnitude[i][j] > magnitude[n1.first][n1.second]) && (magnitude[i][j] > magnitude[n2.first][n2.second]);
}

// Check the two pixels in the direction of the edge (ie, perpendicular to the gradient direction). If the following conditions are satisfied mark them as an edge and return true:
// Have the direction same as the central pixel
// Gradient magnitude is greater than the lower threshold
// They are the maximum compared to their neighbors (nonmaximum suppression for these pixels)
static bool check_neighbours(img::Image<img::Type::GRAYSCALE>& output, int i, int j, const std::vector<std::vector<float>>& magnitude, const std::vector<std::vector<float>>& angles, int lower_threshold)
{
	int angle = angles[i][j];
	std::pair<int, int> n1, n2;
	if (angle == 0) {
		// vertical line
		n1 = {i-1, j};
		n2 = {i+1, j};
	}
	else if (angle == 45) {
		// line is from top right to bottom left (careful here! angles go in the negative direction)
		n1 = {i-1, j+1};
		n2 = {i+1, j-1};
	}
	else if (angle == 90) {
		// horizontal line
		n1 = {i, j-1};
		n2 = {i, j+1};
	}
	else if (angle == 135) {
		// line is from top left to bottom right (careful here! angles go in the negative direction)
		n1 = {i-1, j-1};
		n2 = {i+1, j+1};
	}

	bool ind = false;

	if (output(n1.first, n1.second) == 0 && magnitude[n1.first][n1.second] > lower_threshold) {
		if (angles[n1.first][n1.second] == angle) {
			if (is_maximum(n1.first, n1.second, magnitude, angles)) {
				output(n1.first, n1.second)= ALMOST_WHITE;
				ind = true;
			}
		}
	}

	if (output(n2.first, n2.second) == 0 && magnitude[n2.first][n2.second] > lower_threshold) {
		if (angles[n2.first][n2.second] == angle) {
			if (is_maximum(n2.first, n2.second, magnitude, angles)) {
				output(n2.first, n2.second)= ALMOST_WHITE;
				ind = true;
			}
		}
	}

	return ind;
}

// 5x5 gaussian filter with standard deviation 1.4
static void gaussian_blur(img::Image<img::Type::GRAYSCALE>& img)
{
	std::vector<float> gauss
		{2.0/159, 4.0/159, 5.0/159, 4.0/159, 2.0/159, 
		4.0/159, 9.0/159, 12.0/159, 9.0/159, 4.0/159,
		5.0/159, 12.0/159, 15.0/159, 12.0/159, 5.0/159,
		4.0/159, 9.0/159, 12.0/159, 9.0/159, 4.0/159,
		2.0/159, 4.0/159, 5.0/159, 4.0/159, 2.0/159};

	for (int i = 0; i < img.rows()-4; ++i) {
		for (int j = 0; j < img.cols()-4; ++j) {
			std::vector<float> current;
			current.reserve(25);
			for (int x = 0; x < 5; ++x) {
				for (int y = 0; y < 5; ++y) {
					current.push_back(img(i+x, j+y));
				}
			}

			int s = std::inner_product(gauss.cbegin(), gauss.cend(), current.cbegin(), 0.0);

			img(i+2, j+2) = s;
		}
	}
}

// Get gradient directions and magnitude.
static void sobel_operator_help(const img::Image<img::Type::GRAYSCALE>& img,std::vector<std::vector<float>>& magnitude, std::vector<std::vector<float>>& angles, int from, int to)
{
	std::vector<int> GX{-1, 0, 1, -2, 0, 2, -1, 0, 1};
	std::vector<int> GY{-1, -2, -1, 0, 0, 0, 1, 2, 1};

	for (int i = from; i < to; ++i) {
		for (int j = 0; j < img.cols()-2; ++j) {
			std::vector<int> current{img(i,j), img(i,j+1), img(i,j+2), img(i+1,j), img(i+1,j+1), img(i+1,j+2), img(i+2,j), img(i+2,j+1), img(i+2,j+2)};

			int sx = std::inner_product(GX.cbegin(), GX.cend(), current.cbegin(), 0);
			int sy = std::inner_product(GY.cbegin(), GY.cend(), current.cbegin(), 0);

			magnitude[i+1][j+1] = std::sqrt(sx*sx+sy*sy);

			float theta = std::atan(sy/(double)sx) * 180 / pi;
			if (theta < 0) theta += 180;

			// get gradient direction - there are only 4 directions
			// a line is always perpendicular to the gradient direction
			
			angles[i+1][j+1] = (theta > 112.5 && theta <= 157.5) ? 135
								: (theta > 67.5 && theta <= 112.5) ? 90
								: (theta > 22.5 && theta <= 67.5) ? 45
								: 0;
		}
	}
}

static std::pair<std::vector<std::vector<float>>, std::vector<std::vector<float>>> sobel_operator(const img::Image<img::Type::GRAYSCALE>& img)
{
	std::vector<std::vector<float>> magnitude(img.rows(), std::vector<float>(img.cols(), 0));
	std::vector<std::vector<float>> angles(img.rows(), std::vector<float>(img.cols(), 0));

	int num_threads = 4; // don't do this
	std::vector<std::thread> threads(num_threads);
	int rows = img.rows()-2;
	for (int i = 0; i < num_threads; i++) {
		int from = rows/num_threads * i;
		int to = (i==num_threads-1) ? rows : from + rows/num_threads;
		threads[i] = std::thread(sobel_operator_help, std::ref(img), std::ref(magnitude), std::ref(angles), from, to);
	}

	for (int i = 0; i < num_threads; i++) {
		threads[i].join();
	}

	return {magnitude, angles};
}

// mark pixels as edges (ALMOST_WHITE) if they satisfy conditions
static void nonmaximum_supression_help(img::Image<img::Type::GRAYSCALE>& output, const std::vector<std::vector<float>>& magnitude, const std::vector<std::vector<float>>& angles, int upper_threshold, int from, int to)
{
	for (int i = from; i < to; ++i) {
		for (int j = 1; j < output.cols()-1; ++j) {
			output(i, j) = (magnitude[i][j] >= upper_threshold && is_maximum(i,j,magnitude,angles)) ? ALMOST_WHITE : img::BLACK;
		}
	}
	// remark: we start from 1 because is_maximum access neighbours (+1, -1) and we don't want to check indices (it is faster)
}


static void nonmaximum_supression(img::Image<img::Type::GRAYSCALE>& output, const std::vector<std::vector<float>>& magnitude, const std::vector<std::vector<float>>& angles, int upper_threshold)
{
	int num_threads = 4; // don't do this
	std::vector<std::thread> threads(num_threads);
	int rows = output.rows()-1;
	for (int i = 0; i < num_threads; i++) {
		int from = rows/num_threads * i;
		from = std::max(1, from);
		int to = (i==num_threads-1) ? rows : from + rows/num_threads;
		threads[i] = std::thread(nonmaximum_supression_help, std::ref(output), std::ref(magnitude), std::ref(angles), upper_threshold, from, to);
	}

	for (int i = 0; i < num_threads; i++) {
		threads[i].join();
	}
}

// "grow" lines
static void hysteresis_help(img::Image<img::Type::GRAYSCALE>& output, const std::vector<std::vector<float>>&  magnitude, const std::vector<std::vector<float>>& angles, int lower_threshold, bool& change, int from, int to)
{
	for (int i = from; i < to; ++i) {
		for (int j = 2; j < output.cols()-2; ++j) {
			// if pixel is marked as an edge
			if (output(i,j) == ALMOST_WHITE) {

				// we definitely color marked pixel so we wouldn't check it any more
				output(i,j) = img::WHITE;
				
				// try to mark neighbours as edge
				if (check_neighbours(output, i, j, magnitude, angles, lower_threshold))
					change = true;
			}
		}
	}
	// remark: we start from 2 because check_neighbours access neighbours (+2, -2) and we don't want to check indices (it is faster)
}

static void hysteresis(img::Image<img::Type::GRAYSCALE>& output, const std::vector<std::vector<float>>&  magnitude, const std::vector<std::vector<float>>& angles, int lower_threshold)
{
	bool change = true;

	int num_threads = 4; // don't do this
	std::vector<std::thread> threads(num_threads);
	int rows = output.rows()-2;


	while (change) {
		change = false;

		for (int i = 0; i < num_threads; i++) {
			int from = rows/num_threads * i;
			from = std::max(2, from);
			int to = (i==num_threads-1) ? rows : from + rows/num_threads;
			threads[i] = std::thread(hysteresis_help, std::ref(output), std::ref(magnitude), std::ref(angles), lower_threshold, std::ref(change), from, to);
		}

		for (int i = 0; i < num_threads; i++) {
			threads[i].join();
		}
	}
}


img::Image<img::Type::GRAYSCALE> canny(img::Image<img::Type::GRAYSCALE> img, int lower_threshold = 20, int upper_threshold = 60)
{
	// get rid of a noise
	gaussian_blur(img);

	auto [magnitude, angles] = sobel_operator(img);
	nonmaximum_supression(img, magnitude, angles, upper_threshold);
	hysteresis(img, magnitude, angles, lower_threshold);

	return img;
}


int main()
{
	// auto tstart = std::time(0);
	img::Image<img::Type::GRAYSCALE> img("images/blackboard.jpg");
	if (!img) return -1;

	auto output = canny(img);

	output.show();
	cv::waitKey(0);
	output.save("canny_output.png");
	// std::cout << std::time(0)-tstart << std::endl;

    return 0;
}
