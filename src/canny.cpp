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
	std::vector<std::vector<float>> magnitude(rows, std::vector<float>(columns, 0));
	std::vector<std::vector<float>> angles(rows, std::vector<float>(columns, 0));
	int upper_threshold = 60;
	int lower_threshold = 20;

	std::vector<float> gauss
		{2.0/159, 4.0/159, 5.0/159, 4.0/159, 2.0/159, 
		4.0/159, 9.0/159, 12.0/159, 9.0/159, 4.0/159,
		5.0/159, 12.0/159, 15.0/159, 12.0/159, 5.0/159,
		4.0/159, 9.0/159, 12.0/159, 9.0/159, 4.0/159,
		2.0/159, 4.0/159, 5.0/159, 4.0/159, 2.0/159};

	for (int i = 0; i < rows-3; i++) {
		for (int j = 0; j < columns-3; j++) {
			std::vector<int> current;
			for (int x = 0; x < 5; x++) {
				for (int y = 0; y < 5; y++) {
					current.push_back(img(i+x, j+y));
				}
			}

			int s = std::inner_product(gauss.cbegin(), gauss.cend(), current.cbegin(), 0);

			img(i+2, j+2) = s;
		}
	}


	for (int i = 0; i < rows-2; i++) {
		for (int j = 0; j < columns-2; j++) {
			std::vector<int> current{img(i,j), img(i,j+1), img(i,j+2), img(i+1,j), img(i+1,j+1), img(i+1,j+2), img(i+2,j), img(i+2,j+1), img(i+2,j+2)};

			int sx = std::inner_product(GX.cbegin(), GX.cend(), current.cbegin(), 0);
			int sy = std::inner_product(GY.cbegin(), GY.cend(), current.cbegin(), 0);

			magnitude[i+1][j+1] = std::sqrt(sx*sx+sy*sy);

			float theta = std::atan(sy/(double)sx) * 180 / 3.142;
			while(theta<0) theta+=180;
			angles[i+1][j+1] = theta;
		}
	}

	for (int i = 1; i < rows-1; i++) {
		for (int j = 1; j < columns-1; j++) {
			edges(i,j) = 0;

			if (magnitude[i][j] < upper_threshold) continue;

			if (angles[i][j] > 112.5 && angles[i][j] <= 157.5) {
				// 135 deg
				if (magnitude[i][j] <= magnitude[i-1][j-1]) continue;
				if (magnitude[i][j] <= magnitude[i+1][j+1]) continue;
			}
			else if (angles[i][j] > 67.5 && angles[i][j] <= 112.5) {
				// 90 deg (horizontal line)
				if (magnitude[i][j] <= magnitude[i-1][j]) continue;
				if (magnitude[i][j] <= magnitude[i+1][j]) continue;
			}
			else if (angles[i][j] > 22.5 && angles[i][j] <= 67.5) {
				// 45 deg
				if (magnitude[i][j] <= magnitude[i-1][j+1]) continue;
				if (magnitude[i][j] <= magnitude[i+1][j-1]) continue;
			}
			else {
				// 0 deg (vertical line)
				if (magnitude[i][j] <= magnitude[i][j-1]) continue;
				if (magnitude[i][j] <= magnitude[i][j+1]) continue;
			}

			edges(i,j) = 254;
		}
	}

	bool change = true;
	while (change) {
		change = false;

		for (int i = 1; i < rows-1; i++) {
			for (int j = 1; j < columns-1; j++) {
				if (edges(i,j) == 254) {
					edges(i,j) = 255;

					if (angles[i][j] > 112.5 && angles[i][j] <= 157.5) {
						// 135 deg
						if (edges(i-1, j+1) == 0 && magnitude[i-1][j+1] > lower_threshold) {
							if (angles[i-1][j+1] > 112.5 && angles[i-1][j+1] <= 157.5) {
								if (i-2<=0 || magnitude[i-1][j+1] > magnitude[i-2][j]) {
									if (j+2>=columns || magnitude[i-1][j+1] > magnitude[i][j+2]) {
										edges(i-1,j+1)=254;
										change = true;
									}
								}
							}
						}


						if (edges(i+1, j-1) == 0 && magnitude[i+1][j-1] > lower_threshold) {
							if (angles[i+1][j-1] > 112.5 && angles[i+1][j-1] <= 157.5) {
								if (i+2 >= columns || magnitude[i+1][j-1] > magnitude[i+2][j]) {
									if (j-2 <= 0 || magnitude[i+1][j-1] > magnitude[i][j-2]) {
										edges(i+1,j-1)=254;
										change = true;
									}
								}
							}
						}
					} // end of 135


					if (angles[i][j] > 67.5 && angles[i][j] <= 112.5) {
						// 90 deg (horizontal line)
						if (edges(i, j+1) == 0 && magnitude[i][j+1] > lower_threshold) {
							if (angles[i][j+1] > 67.5 && angles[i][j+1] <= 112.5) {
								if (magnitude[i][j+1] > magnitude[i-1][j+1]) {
									if (magnitude[i][j+1] > magnitude[i+1][j+1]) {
										edges(i,j+1)=254;
										change = true;
									}
								}
							}
						}


						if (edges(i, j-1) == 0 && magnitude[i][j-1] > lower_threshold) {
							if (angles[i][j-1] > 67.5 && angles[i][j-1] <= 112.5) {
								if (magnitude[i][j-1] > magnitude[i-1][j-1]) {
									if (magnitude[i][j-1] > magnitude[i+1][j-1]) {
										edges(i,j-1)=254;
										change = true;
									}
								}
							}
						}
					} // end of 90


					if (angles[i][j] > 22.5 && angles[i][j] <= 67.5) {
						// 45 deg
						if (edges(i-1, j-1) == 0 && magnitude[i-1][j-1] > lower_threshold) {
							if (angles[i-1][j-1] > 22.5 && angles[i-1][j-1] <= 67.5) {
								if (i-2<=0 || magnitude[i-1][j-1] > magnitude[i-2][j]) {
									if (j-2<=0 || magnitude[i-1][j-1] > magnitude[i][j-2]) {
										edges(i-1,j-1)=254;
										change = true;
									}
								}
							}
						}

						if (edges(i+1, j+1) == 0 && magnitude[i+1][j+1] > lower_threshold) {
							if (angles[i+1][j+1] > 22.5 && angles[i+1][j+1] <= 67.5) {
								if (i+2 >= rows || magnitude[i+1][j+1] > magnitude[i+2][j]) {
									if (j+2 >= columns || magnitude[i+1][j+1] > magnitude[i][j+2]) {
										edges(i+1,j+1)=254;
										change = true;
									}
								}
							}
						}
					} // end of 45

					if (angles[i][j] <= 22.5) {
						// 0 deg (vertical line)
						if (edges(i+1, j) == 0 && magnitude[i+1][j] > lower_threshold) {
							if (angles[i+1][j] <= 22.5) {
								if (magnitude[i+1][j] > magnitude[i+1][j-1]) {
									if (magnitude[i+1][j] > magnitude[i+1][j+1]) {
										edges(i+1,j)=254;
										change = true;
									}
								}
							}
						}

						if (edges(i-1, j) == 0 && magnitude[i-1][j] > lower_threshold) {
							if (angles[i-1][j] <= 22.5) {
								if (magnitude[i-1][j] > magnitude[i-1][j-1]) {
									if (magnitude[i-1][j] > magnitude[i-1][j+1]) {
										edges(i-1,j)=254;
										change = true;
									}
								}
							}
						}


					} // end of 0

				}
			}
		}
	}

	edges.show();
	cv::waitKey(0);
	edges.save("canny_output.png");

    return 0;
}
