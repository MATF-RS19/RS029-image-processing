#include "image.hpp"
#include <vector>
#include <numeric>

static const int ALMOST_WHITE = 254;

static const double pi = 3.14159265358979323846;

using pixel = std::pair<int,int>;

class canny_output {
public:
	std::vector<std::vector<float>> magnitude;
	std::vector<std::vector<float>> angles;
	img::Image<img::Type::GRAYSCALE> output;
	const img::Image<img::Type::GRAYSCALE>& img;

	canny_output(const img::Image<img::Type::GRAYSCALE>& img)
		: magnitude(img.rows(), std::vector<float>(img.cols(), 0)),
		  angles(img.rows(), std::vector<float>(img.cols(), 0)),
		  output(img.rows(), img.cols()),
		  img(img)
	{}
};

// Check two pixels that belong to the parallel lines below and above the line of a pixel (i,j). If magnitude of pixel (i,j) is greater than the magnitude of those two pixels return true.
static bool is_maximum(canny_output& c, const pixel& p)
{
	auto&& [i, j] = p;
	int angle = c.angles[i][j];
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

	return (c.magnitude[i][j] > c.magnitude[n1.first][n1.second]) && (c.magnitude[i][j] > c.magnitude[n2.first][n2.second]);
}

// Check the two pixels in the direction of the edge (ie, perpendicular to the gradient direction). If the following conditions are satisfied mark them as an edge and return true:
// Have the direction same as the central pixel
// Gradient magnitude is greater than the lower threshold
// They are the maximum compared to their neighbors (nonmaximum suppression for these pixels)
static bool check_neighbours(canny_output& c, const pixel& p, int lower_threshold)
{
	auto&& [i, j] = p;

	int angle = c.angles[i][j];
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

	if (c.output(n1.first, n1.second) == 0 && c.magnitude[n1.first][n1.second] > lower_threshold) {
		if (c.angles[n1.first][n1.second] == angle) {
			if (is_maximum(c, n1)) {
				c.output(n1.first, n1.second)= ALMOST_WHITE;
				ind = true;
			}
		}
	}

	if (c.output(n2.first, n2.second) == 0 && c.magnitude[n2.first][n2.second] > lower_threshold) {
		if (c.angles[n2.first][n2.second] == angle) {
			if (is_maximum(c, n2)) {
				c.output(n2.first, n2.second)= ALMOST_WHITE;
				ind = true;
			}
		}
	}

	return ind;
}

// 5x5 gaussian filter with standard deviation 1.4
static void gaussian_blur_help(canny_output& c, const std::vector<float>& gauss, int from, int to)
{
	for (int i = from; i < to; ++i) {
		for (int j = 0; j < c.img.cols()-4; ++j) {
			std::vector<float> current;
			current.reserve(25);
			for (int x = 0; x < 5; ++x) {
				for (int y = 0; y < 5; ++y) {
					current.push_back(c.img(i+x, j+y));
				}
			}

			int s = std::inner_product(gauss.cbegin(), gauss.cend(), current.cbegin(), 0.0);

			c.output(i+2, j+2) = s;
		}
	}
}

static void gaussian_blur(canny_output& c)
{
	std::vector<float> gauss
		{2.0/159, 4.0/159, 5.0/159, 4.0/159, 2.0/159, 
		4.0/159, 9.0/159, 12.0/159, 9.0/159, 4.0/159,
		5.0/159, 12.0/159, 15.0/159, 12.0/159, 5.0/159,
		4.0/159, 9.0/159, 12.0/159, 9.0/159, 4.0/159,
		2.0/159, 4.0/159, 5.0/159, 4.0/159, 2.0/159};

	img::start_threads(0, c.img.rows()-4, gaussian_blur_help, c, gauss);
}

// Get gradient directions and magnitude.
static void sobel_operator_help(canny_output& c, int from, int to)
{
	std::vector<int> GX{-1, 0, 1, -2, 0, 2, -1, 0, 1};
	std::vector<int> GY{-1, -2, -1, 0, 0, 0, 1, 2, 1};

	for (int i = from; i < to; ++i) {
		for (int j = 0; j < c.img.cols()-2; ++j) {
			std::vector<int> current{c.output(i,j), c.output(i,j+1), c.output(i,j+2), c.output(i+1,j), c.output(i+1,j+1), c.output(i+1,j+2), c.output(i+2,j), c.output(i+2,j+1), c.output(i+2,j+2)};

			int sx = std::inner_product(GX.cbegin(), GX.cend(), current.cbegin(), 0);
			int sy = std::inner_product(GY.cbegin(), GY.cend(), current.cbegin(), 0);

			c.magnitude[i+1][j+1] = std::sqrt(sx*sx+sy*sy);

			float theta = std::atan(sy/(double)sx) * 180 / pi;
			if (theta < 0) theta += 180;

			// get gradient direction - there are only 4 directions
			// a line is always perpendicular to the gradient direction
			
			c.angles[i+1][j+1] = (theta > 112.5 && theta <= 157.5) ? 135
								: (theta > 67.5 && theta <= 112.5) ? 90
								: (theta > 22.5 && theta <= 67.5) ? 45
								: 0;
		}
	}
}


static void sobel_operator(canny_output& c)
{
	img::start_threads(0, c.img.rows()-2, sobel_operator_help, c);
}

// mark pixels as edges (ALMOST_WHITE) if they satisfy conditions
static void nonmaximum_supression_help(canny_output& c, int upper_threshold, int from, int to)
{
	for (int i = from; i < to; ++i) {
		for (int j = 1; j < c.output.cols()-1; ++j) {
			c.output(i, j) = (c.magnitude[i][j] >= upper_threshold && is_maximum(c, {i, j})) ? ALMOST_WHITE : img::BLACK;
		}
	}
	// remark: we start from 1 because is_maximum access neighbours (+1, -1) and we don't want to check indices (it is faster)
}


static void nonmaximum_supression(canny_output& c, int upper_threshold)
{
	img::start_threads(1, c.output.rows()-1, nonmaximum_supression_help, c, upper_threshold);
}

// "grow" lines
static void hysteresis_help(canny_output& c, int lower_threshold, bool& change, int from, int to)
{
	for (int i = from; i < to; ++i) {
		for (int j = 2; j < c.output.cols()-2; ++j) {
			// if pixel is marked as an edge
			if (c.output(i,j) == ALMOST_WHITE) {

				// we definitely color marked pixel so we wouldn't check it any more
				c.output(i,j) = img::WHITE;
				
				// try to mark neighbours as edge
				if (check_neighbours(c, {i, j}, lower_threshold))
					change = true;
			}
		}
	}
	// remark: we start from 2 because check_neighbours access neighbours (+2, -2) and we don't want to check indices (it is faster)
}

static void hysteresis(canny_output& c, int lower_threshold)
{
	bool change = true;

	while (change) {
		change = false;

		img::start_threads(2, c.output.rows()-2, hysteresis_help, c, lower_threshold, change);
	}
}


img::Image<img::Type::GRAYSCALE> canny(const img::Image<img::Type::GRAYSCALE>& img, int lower_threshold = 20, int upper_threshold = 60)
{
	canny_output c(img);
	gaussian_blur(c);
	sobel_operator(c);
	nonmaximum_supression(c, upper_threshold);
	hysteresis(c, lower_threshold);

	for (int i = 0; i < c.output.rows(); i++) {
		for (int j = 0; j < 4; j++) {
			c.output(i, j) = c.output(i, c.output.cols()-1-j) = img::Color::BLACK;
		}
	}

	for (int j = 0; j < c.output.cols(); j++) {
		for (int i = 0; i < 4; i++) {
			c.output(i, j) = c.output(c.output.rows()-1-i, j) = img::Color::BLACK;
		}
	}

	return c.output;
}


int main()
{
		

	auto tstart = std::time(0);
	// for (int i = 0; i < 30; i++) {
	img::Image<img::Type::GRAYSCALE> img("images/sobel.png");
	if (!img) return -1;
		auto output = canny(img);
	// }

	output.show();
	cv::waitKey(0);
	output.save("canny_output.png");
	std::cout << std::time(0)-tstart << std::endl;

    return 0;
}
