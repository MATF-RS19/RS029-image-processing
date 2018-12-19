#include "image.hpp"
#include <vector>
#include <numeric>

static const double pi = 3.14159265358979323846;

class canny_detector {
public:
	canny_detector(const img::Image<img::Type::GRAYSCALE>& img)
		: m_magnitude(img.rows(), std::vector<float>(img.cols(), 0)),
		  m_angles(img.rows(), std::vector<float>(img.cols(), 0)),
		  m_output(img.rows(), img.cols()),
		  m_img(img)
	{}

	const img::Image<img::Type::GRAYSCALE>& canny(int lower_threshold = 20, int upper_threshold = 60)
	{
		gaussian_blur();
		sobel_operator();
		nonmaximum_supression(upper_threshold);
		hysteresis(lower_threshold);

		for (int i = 0; i < m_output.rows(); i++) {
			for (int j = 0; j < 4; j++) {
				m_output(i, j) = m_output(i, m_output.cols()-1-j) = img::Color::BLACK;
			}
		}

		for (int j = 0; j < m_output.cols(); j++) {
			for (int i = 0; i < 4; i++) {
				m_output(i, j) = m_output(m_output.rows()-1-i, j) = img::Color::BLACK;
			}
		}

		return m_output;
	}

private:
	using pixel = std::pair<int,int>;
	const int ALMOST_WHITE = 254;

	std::vector<std::vector<float>> m_magnitude;
	std::vector<std::vector<float>> m_angles;
	img::Image<img::Type::GRAYSCALE> m_output;
	const img::Image<img::Type::GRAYSCALE>& m_img;

	// 5x5 gaussian filter with standard deviation 1.4
	void gaussian_blur_help(const std::vector<float>& gauss, int from, int to)
	{
		for (int i = from; i < to; ++i) {
			for (int j = 0; j < m_img.cols()-4; ++j) {
				std::vector<float> current;
				current.reserve(25);
				for (int x = 0; x < 5; ++x) {
					for (int y = 0; y < 5; ++y) {
						current.push_back(m_img(i+x, j+y));
					}
				}

				int s = std::inner_product(gauss.cbegin(), gauss.cend(), current.cbegin(), 0.0);

				m_output(i+2, j+2) = s;
			}
		}
	}

	void gaussian_blur()
	{
		std::vector<float> gauss
			{2.0/159, 4.0/159, 5.0/159, 4.0/159, 2.0/159, 
			4.0/159, 9.0/159, 12.0/159, 9.0/159, 4.0/159,
			5.0/159, 12.0/159, 15.0/159, 12.0/159, 5.0/159,
			4.0/159, 9.0/159, 12.0/159, 9.0/159, 4.0/159,
			2.0/159, 4.0/159, 5.0/159, 4.0/159, 2.0/159};

		img::start_threads(0, m_img.rows()-4, &canny_detector::gaussian_blur_help, this, gauss);
	}

	// Get gradient directions and m_magnitude.
	void sobel_operator_help(int from, int to)
	{
		std::vector<int> GX{-1, 0, 1, -2, 0, 2, -1, 0, 1};
		std::vector<int> GY{-1, -2, -1, 0, 0, 0, 1, 2, 1};

		for (int i = from; i < to; ++i) {
			for (int j = 0; j < m_img.cols()-2; ++j) {
				std::vector<int> current{m_output(i,j), m_output(i,j+1), m_output(i,j+2), m_output(i+1,j), m_output(i+1,j+1), m_output(i+1,j+2), m_output(i+2,j), m_output(i+2,j+1), m_output(i+2,j+2)};

				int sx = std::inner_product(GX.cbegin(), GX.cend(), current.cbegin(), 0);
				int sy = std::inner_product(GY.cbegin(), GY.cend(), current.cbegin(), 0);

				m_magnitude[i+1][j+1] = std::sqrt(sx*sx+sy*sy);

				float theta = std::atan(sy/(double)sx) * 180 / pi;
				if (theta < 0) theta += 180;

				// get gradient direction - there are only 4 directions
				// a line is always perpendicular to the gradient direction
				
				m_angles[i+1][j+1] = (theta > 112.5 && theta <= 157.5) ? 135
									: (theta > 67.5 && theta <= 112.5) ? 90
									: (theta > 22.5 && theta <= 67.5) ? 45
									: 0;
			}
		}
	}

	void sobel_operator()
	{
		img::start_threads(0, m_img.rows()-2, &canny_detector::sobel_operator_help, this);
	}

	// Check two pixels that belong to the parallel lines below and above the line of a pixel (i,j). If m_magnitude of pixel (i,j) is greater than the m_magnitude of those two pixels return true.
	bool is_maximum(const pixel& p) const
	{
		auto&& [i, j] = p;
		int angle = m_angles[i][j];
		std::pair<int, int> n1, n2;
		if (angle == 0) {
			// vertical line
			n1 = {i, j-1};
			n2 = {i, j+1};
		}
		else if (angle == 45) {
			// line is from top right to bottom left (careful here! m_angles go in the negative direction)
			n1 = {i-1, j-1};
			n2 = {i+1, j+1};
		}
		else if (angle == 90) {
			// horizontal line
			n1 = {i-1, j};
			n2 = {i+1, j};
		}
		else if (angle == 135) {
			// line is from top left to bottom right (careful here! m_angles go in the negative direction)
			n1 = {i-1, j+1};
			n2 = {i+1, j-1};
		}

		return (m_magnitude[i][j] > m_magnitude[n1.first][n1.second]) && (m_magnitude[i][j] > m_magnitude[n2.first][n2.second]);
	}

	// mark pixels as edges (ALMOST_WHITE) if they satisfy conditions
	void nonmaximum_supression_help(int upper_threshold, int from, int to)
	{
		for (int i = from; i < to; ++i) {
			for (int j = 1; j < m_output.cols()-1; ++j) {
				m_output(i, j) = (m_magnitude[i][j] >= upper_threshold && is_maximum({i, j})) ? ALMOST_WHITE : img::BLACK;
			}
		}
		// remark: we start from 1 because is_maximum access neighbours (+1, -1) and we don't want to check indices (it is faster)
	}


	void nonmaximum_supression(int upper_threshold)
	{
		img::start_threads(1, m_output.rows()-1, &canny_detector::nonmaximum_supression_help, this, upper_threshold);
	}

	// Check the two pixels in the direction of the edge (ie, perpendicular to the gradient direction). If the following conditions are satisfied mark them as an edge and return true:
	// Have the direction same as the central pixel
	// Gradient m_magnitude is greater than the lower threshold
	// They are the maximum compared to their neighbors (nonmaximum suppression for these pixels)
	bool check_neighbours(const pixel& p, int lower_threshold)
	{
		auto&& [i, j] = p;

		int angle = m_angles[i][j];
		std::pair<int, int> n1, n2;
		if (angle == 0) {
			// vertical line
			n1 = {i-1, j};
			n2 = {i+1, j};
		}
		else if (angle == 45) {
			// line is from top right to bottom left (careful here! m_angles go in the negative direction)
			n1 = {i-1, j+1};
			n2 = {i+1, j-1};
		}
		else if (angle == 90) {
			// horizontal line
			n1 = {i, j-1};
			n2 = {i, j+1};
		}
		else if (angle == 135) {
			// line is from top left to bottom right (careful here! m_angles go in the negative direction)
			n1 = {i-1, j-1};
			n2 = {i+1, j+1};
		}

		bool ind = false;

		if (m_output(n1.first, n1.second) == 0 && m_magnitude[n1.first][n1.second] > lower_threshold) {
			if (m_angles[n1.first][n1.second] == angle) {
				if (is_maximum(n1)) {
					m_output(n1.first, n1.second)= ALMOST_WHITE;
					ind = true;
				}
			}
		}

		if (m_output(n2.first, n2.second) == 0 && m_magnitude[n2.first][n2.second] > lower_threshold) {
			if (m_angles[n2.first][n2.second] == angle) {
				if (is_maximum(n2)) {
					m_output(n2.first, n2.second)= ALMOST_WHITE;
					ind = true;
				}
			}
		}

		return ind;
	}

	// "grow" lines
	void hysteresis_help(int lower_threshold, bool& change, int from, int to)
	{
		for (int i = from; i < to; ++i) {
			for (int j = 2; j < m_output.cols()-2; ++j) {
				// if pixel is marked as an edge
				if (m_output(i,j) == ALMOST_WHITE) {

					// we definitely color marked pixel so we wouldn't check it any more
					m_output(i,j) = img::WHITE;
					
					// try to mark neighbours as edge
					if (check_neighbours({i, j}, lower_threshold))
						change = true;
				}
			}
		}
		// remark: we start from 2 because check_neighbours access neighbours (+2, -2) and we don't want to check indices (it is faster)
	}

	void hysteresis(int lower_threshold)
	{
		bool change = true;

		while (change) {
			change = false;

			img::start_threads(2, m_output.rows()-2, &canny_detector::hysteresis_help, this, lower_threshold, change);
		}
	}
};



img::Image<img::Type::GRAYSCALE> canny(const img::Image<img::Type::GRAYSCALE>& img, int lower_threshold, int upper_threshold)
{
	return canny_detector(img).canny(lower_threshold, upper_threshold);
}

