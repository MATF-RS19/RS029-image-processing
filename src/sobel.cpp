#include "image.hpp"
#include <vector>
#include <numeric>

int main()
{
	img::Image<img::Type::GRAYSCALE> img("images/sobel.png");
	int rows = img.rows();
	int columns = img.cols();
	img::Image<img::Type::GRAYSCALE> edges(rows, columns);

	std::vector<int> GX{-1, 0, 1, -2, 0, 2, -1, 0, 1};
	std::vector<int> GY{-1, -2, -1, 0, 0, 0, 1, 2, 1};

	for (int i = 0; i < rows-2; i++) {
		for (int j = 0; j < columns-2; j++) {
			std::vector<int> current{img(i,j), img(i,j+1), img(i,j+2), img(i+1,j), img(i+1,j+1), img(i+1,j+2), img(i+2,j), img(i+2,j+1), img(i+2,j+2)};
			int s1 = std::inner_product(GX.cbegin(), GX.cend(), current.cbegin(), 0);
			int s2 = std::inner_product(GY.cbegin(), GY.cend(), current.cbegin(), 0);
			int s = std::sqrt(s1*s1+s2*s2);
			edges(i+1,j+1)=s;
			// edges(i+1, j+1) = (s > 70) ? 255-s : 255;
		}
	}

	edges.show();
	cv::waitKey(0);
	edges.save("sobel_output.png");

    return 0;
}
