#include "image.hpp"
#include <vector>
#include <numeric>
#include <stack>
#include <time.h>

using MatrixF = std::vector<std::vector<float>>;
using MatrixI = std::vector<std::vector<int>>;
static const double pi = 3.14159265358979323846;

static void gaussian_blur(img::Image<img::Type::GRAYSCALE>& img)
{
	std::vector<float> gauss
		{2.0/159, 4.0/159, 5.0/159, 4.0/159, 2.0/159, 
		4.0/159, 9.0/159, 12.0/159, 9.0/159, 4.0/159,
		5.0/159, 12.0/159, 15.0/159, 12.0/159, 5.0/159,
		4.0/159, 9.0/159, 12.0/159, 9.0/159, 4.0/159,
		2.0/159, 4.0/159, 5.0/159, 4.0/159, 2.0/159};

	for (int i = -2; i < img.rows()-2; i++) {
		for (int j = -2; j < img.cols()-2; j++) {
			std::vector<int> current;
			for (int x = 0; x < 5; x++) {
				for (int y = 0; y < 5; y++) {
					current.push_back(img.in_range(i+x, j+y) ? img(i+x, j+y) : 0);
				}
			}

			img(i+2, j+2) = std::inner_product(gauss.cbegin(), gauss.cend(), current.cbegin(), 0.0);
		}
	}
}


static std::pair<MatrixF, MatrixF> sobel_operator(const img::Image<img::Type::GRAYSCALE>& img)
{
	MatrixF magnitude(img.rows(), std::vector<float>(img.cols(), 0));
	MatrixF angles(img.rows(), std::vector<float>(img.cols(), 0));
	std::vector<int> GX{-1, 0, 1, -2, 0, 2, -1, 0, 1};
	std::vector<int> GY{-1, -2, -1, 0, 0, 0, 1, 2, 1};

	for (int i = -1; i < (int)img.rows()-1; i++) {
		for (int j = -1; j < (int)img.cols()-1; j++) {
			std::vector<float> current;
			for (int x = 0; x < 3; x++) {
				for (int y = 0; y < 3; y++) {
					current.push_back((img.in_range(i+x, j+y)) ? img(i+x, j+y) : 0);
				}
			}

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

	return {magnitude, angles};
}


static bool is_maximum(int i, int j, const MatrixF& magnitude, const MatrixF& angles)
{
	int rows = magnitude.size();
	int cols = magnitude.front().size();

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

	return (n1.first < 0 || n1.first >= rows || n1.second < 0 || n1.second >= cols || magnitude[i][j] > magnitude[n1.first][n1.second])
		&& (n2.first < 0 || n2.first >= rows || n2.second < 0 || n2.second >= cols || magnitude[i][j] > magnitude[n2.first][n2.second]);

}

static void nonmaximum_supression(img::Image<img::Type::GRAYSCALE>& outputs, const MatrixF& magnitude, const MatrixF& angles, int upper_threshold)
{
	for (int i = 0; i < outputs.rows(); i++) {
		for (int j = 0; j < outputs.cols(); j++) {
			outputs(i,j) = (magnitude[i][j] < upper_threshold || !is_maximum(i, j, magnitude, angles)) ? img::BLACK : img::WHITE;
		}
	}
}

static void hysterysis(img::Image<img::Type::GRAYSCALE>& outputs, int i, int j, const MatrixF& magnitude, const MatrixF& angles, int lower_threshold, std::stack<std::pair<int,int>>& st)
{
	auto [rows, cols] = outputs.dimension();
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

	if (n1.first >= 0 && n1.first < rows && n1.second >= 0 && n1.second < cols) {
		if (outputs(n1.first, n1.second) == 0 && magnitude[n1.first][n1.second] > lower_threshold) {
			if (angles[n1.first][n1.second] == angle) {
				if (is_maximum(n1.first, n1.second, magnitude, angles)) {
					outputs(n1.first, n1.second)= img::WHITE;
					st.push(n1);
				}
			}
		}
	}

	if (n2.first >= 0 && n2.first < rows && n2.second >= 0 && n2.second < cols) {
		if (outputs(n2.first, n2.second) == 0 && magnitude[n2.first][n2.second] > lower_threshold) {
			if (angles[n2.first][n2.second] == angle) {
				if (is_maximum(n2.first, n2.second, magnitude, angles)) {
					outputs(n2.first, n2.second)= img::WHITE;
					st.push(n2);
				}
			}
		}
	}
}

img::Image<img::Type::GRAYSCALE> canny_edge_detector(img::Image<img::Type::GRAYSCALE> img, int lower_threshold = 20, int upper_threshold = 60)
{
	auto [rows, cols] = img.dimension();

	gaussian_blur(img);

	auto [magnitude, angles] = sobel_operator(img);

	nonmaximum_supression(img, magnitude, angles, upper_threshold);

	std::stack<std::pair<int,int>> st;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (img(i,j) == img::WHITE) {
				hysterysis(img, i, j, magnitude, angles, lower_threshold, st);
			}
		}
	}

	while (!st.empty()) {
		auto [i, j] = st.top();
		st.pop();
		hysterysis(img, i, j, magnitude, angles, lower_threshold, st);
	}

	return img;
}


int main()
{
	img::Image<img::Type::GRAYSCALE> img("images/sobel.png");
	if (!img) return -1;

	auto output = canny_edge_detector(img);

	output.show();
	cv::waitKey(0);
	output.save("canny_output.png");

    return 0;
}
