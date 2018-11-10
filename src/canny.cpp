#include "image.hpp"
#include <vector>
#include <numeric>
#include <stack>
static const double pi = 3.14159265358979323846;

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

bool changed(img::Image<img::Type::GRAYSCALE>& edges, int i, int j, const std::vector<std::vector<float>>& magnitude, const std::vector<std::vector<float>>& angles, int lower_threshold)
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

	if (edges(n1.first, n1.second) == 0 && magnitude[n1.first][n1.second] > lower_threshold) {
		if (angles[n1.first][n1.second] == angle) {
			if (is_maximum(n1.first, n1.second, magnitude, angles)) {
				edges(n1.first, n1.second)= 254;
				ind = true;
			}
		}
	}

	if (edges(n2.first, n2.second) == 0 && magnitude[n2.first][n2.second] > lower_threshold) {
		if (angles[n2.first][n2.second] == angle) {
			if (is_maximum(n2.first, n2.second, magnitude, angles)) {
				edges(n2.first, n2.second)= 254;
				ind = true;
			}
		}
	}


	return ind;
}

static void gaussian_blur(img::Image<img::Type::GRAYSCALE>& img)
{
	std::vector<float> gauss
		{2.0/159, 4.0/159, 5.0/159, 4.0/159, 2.0/159, 
		4.0/159, 9.0/159, 12.0/159, 9.0/159, 4.0/159,
		5.0/159, 12.0/159, 15.0/159, 12.0/159, 5.0/159,
		4.0/159, 9.0/159, 12.0/159, 9.0/159, 4.0/159,
		2.0/159, 4.0/159, 5.0/159, 4.0/159, 2.0/159};

	for (int i = 0; i < img.rows()-4; i++) {
		for (int j = 0; j < img.cols()-4; j++) {
			std::vector<float> current;
			for (int x = 0; x < 5; x++) {
				for (int y = 0; y < 5; y++) {
					current.push_back(img(i+x, j+y));
				}
			}

			int s = std::inner_product(gauss.cbegin(), gauss.cend(), current.cbegin(), 0.0);

			img(i+2, j+2) = s;
		}
	}
}

static std::pair<std::vector<std::vector<float>>, std::vector<std::vector<float>>> sobel_operator(const img::Image<img::Type::GRAYSCALE>& img)
{
	std::vector<std::vector<float>> magnitude(img.rows(), std::vector<float>(img.cols(), 0));
	std::vector<std::vector<float>> angles(img.rows(), std::vector<float>(img.cols(), 0));
	std::vector<int> GX{-1, 0, 1, -2, 0, 2, -1, 0, 1};
	std::vector<int> GY{-1, -2, -1, 0, 0, 0, 1, 2, 1};

	for (int i = 0; i < img.rows()-2; i++) {
		for (int j = 0; j < img.cols()-2; j++) {
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

	return {magnitude, angles};
}

static void nonmaximum_supression(img::Image<img::Type::GRAYSCALE>& edges, const std::vector<std::vector<float>>& magnitude, const std::vector<std::vector<float>>& angles, int upper_threshold)
{
	for (int i = 1; i < edges.rows()-1; i++) {
		for (int j = 1; j < edges.cols()-1; j++) {
			edges(i,j) = 0;
			if (magnitude[i][j] < upper_threshold) continue;
			if (!is_maximum(i,j,magnitude,angles)) continue;
			edges(i,j) = 254;
		}
	}
}

int main()
{
	clock_t tStart = clock();
	for (int k = 0; k < 1; k++) {
		// img::Image<img::Type::GRAYSCALE> img("images/mat.jpg");
		img::Image<img::Type::GRAYSCALE> img("images/sobel.png");
		int rows = img.rows();
		int columns = img.cols();
		img::Image<img::Type::GRAYSCALE> edges(rows, columns);

		int upper_threshold = 60;
		int lower_threshold = 20;

		gaussian_blur(img);

		auto [magnitude, angles] = sobel_operator(img);

		nonmaximum_supression(edges, magnitude, angles, upper_threshold);

		bool change = true;
		while (change) {
			change = false;

			for (int i = 2; i < rows-2; i++) {
				for (int j = 2; j < columns-2; j++) {
					if (edges(i,j) == 254) {
						edges(i,j) = 255;

						bool tmp = changed(edges, i, j,  magnitude,  angles, lower_threshold);
						if (change == false)
							change = tmp;
					}
				}
			}
		}
		edges.show();
		cv::waitKey(0);
		edges.save("canny_output.png");
	}
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    return 0;
}
