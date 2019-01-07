#include "image.hpp"
using namespace img;

#define MAX_COLOR_LEVEL 255

template<>
void Image<Type::RGB>::bgr2rgb()
{
    cv::cvtColor(m_data, m_data, CV_BGR2RGB);
}

template <>
Image<Type::RGB>::Image(const std::experimental::filesystem::path& path)
    : m_name(path.filename()), m_data(cv::imread(path.string(), cv::IMREAD_COLOR))
{
}

template <>
Image<Type::GRAYSCALE>::Image(const std::experimental::filesystem::path& path)
    : m_name(path.filename()), m_data(cv::imread(path.string(), cv::IMREAD_GRAYSCALE))
{
}

template <>
Image<Type::RGB>::Image(unsigned rows, unsigned cols, Color c, std::string name)
    : m_name(std::move(name)), m_data(cv::Mat(rows, cols, CV_8UC3, cv::Scalar(c, c, c)))
{
}

template <>
Image<Type::GRAYSCALE>::Image(unsigned rows, unsigned cols, Color c, std::string name)
    : m_name(std::move(name)), m_data(cv::Mat(rows, cols, CV_8UC1, cv::Scalar(c)))
{
}

template<>
unsigned Image<Type::RGB>::pixel_sum(unsigned i, unsigned j) const
{
    auto& x = m_data.at<cv::Vec3b>(i, j);
    return x[R] + x[G] + x[B];
}

template<>
unsigned Image<Type::GRAYSCALE>::pixel_sum(unsigned i, unsigned j) const
{
    return m_data.at<unsigned char>(i,j);
}

template<>
Image<Type::GRAYSCALE> Image<Type::RGB>::grayscale() const
{
    img::Image<img::Type::GRAYSCALE> gray(rows(), cols());

    for (int i = 0; i < rows(); i++) {
        for (int j = 0; j < cols(); j++) {
            gray(i, j) = std::round(0.299*(*this)(i, j)[R] + 0.587*(*this)(i, j)[G] + 0.114*(*this)(i, j)[B]);
        }
    }

    return gray;
}

template<>
Image<Type::GRAYSCALE> Image<Type::GRAYSCALE>::black_white() const
{
    img::Image<img::Type::GRAYSCALE> result(rows(), cols());

    double threshold = (double)std::accumulate(cbegin(), cend(), 0) / (rows()*cols());

    for (int i = 0; i < rows(); i++) {
        for (int j = 0; j < cols(); j++) {
            result(i, j) = (pixel_sum(i, j) < threshold) ? Color::BLACK : Color::WHITE;
        }
    }

    return result;
}

template<>
Image<Type::GRAYSCALE> Image<Type::RGB>::black_white() const
{
    return grayscale().black_white();
}

// 5x5 gaussian filter with standard deviation 1.4
template<>
void Image<Type::GRAYSCALE>::gaussian_blur_help(Image<Type::GRAYSCALE>& output, const std::vector<float>& gauss, int from, int to) const
{
	const int gauss_filter_size = 5;
	for (int i = from; i < to; ++i) {
		for (int j = 0; j <= cols()-gauss_filter_size; ++j) {
			std::vector<float> current;
			current.reserve(gauss_filter_size*gauss_filter_size);
			for (int x = 0; x < gauss_filter_size; ++x) {
				for (int y = 0; y < gauss_filter_size; ++y) {
					current.push_back((*this)(i+x, j+y));
				}
			}

			output(i + gauss_filter_size/2, j + gauss_filter_size/2) = std::inner_product(gauss.cbegin(), gauss.cend(), current.cbegin(), 0.0);
		}
	}
}

template<>
Image<Type::GRAYSCALE> Image<Type::GRAYSCALE>::gaussian_blur() const
{
	const int gauss_filter_size = 5;
	const std::vector<float> gauss
		{2.0/159, 4.0/159, 5.0/159, 4.0/159, 2.0/159, 
		4.0/159, 9.0/159, 12.0/159, 9.0/159, 4.0/159,
		5.0/159, 12.0/159, 15.0/159, 12.0/159, 5.0/159,
		4.0/159, 9.0/159, 12.0/159, 9.0/159, 4.0/159,
		2.0/159, 4.0/159, 5.0/159, 4.0/159, 2.0/159};

	Image<Type::GRAYSCALE> output(rows(), cols());

	img::start_threads(0, rows()-gauss_filter_size+1, &Image::gaussian_blur_help, this, output, gauss);

	output.set_borders(gauss_filter_size/2, *this);

	return output;
}

template<>
unsigned Image<Type::RGB>::red(unsigned i, unsigned j) const
{
	return m_data.at<cv::Vec3b>(i, j)[R];
}

template<>
unsigned Image<Type::RGB>::green(unsigned i, unsigned j) const
{
	return m_data.at<cv::Vec3b>(i, j)[G];
}

template<>
unsigned Image<Type::RGB>::blue(unsigned i, unsigned j) const
{
	return m_data.at<cv::Vec3b>(i, j)[B];
}

// Samarth Jain, Megha Soni
// "Digital Image Quality Enhancement Using Compound Contrast Enhancement Process"
// IJEDR, Volume 6, Issue 3
// https://www.ijedr.org/papers/IJEDR1803071.pdf

template<>
Image<Type::GRAYSCALE> Image<Type::GRAYSCALE>::negative() const
{
	Image<Type::GRAYSCALE> output(rows(), cols());

    for (int i = 0; i < rows(); i++) {
        for (int j = 0; j <  cols(); ++j) {
            output(i, j) = MAX_COLOR_LEVEL - (*this)(i, j); 
        }
    }

	return output;
}

template<>
Image<Type::RGB> Image<Type::RGB>::negative() const
{
    Image<Type::RGB> output(rows(), cols());

    for (int i = 0; i < rows(); i++) {
        for (int j = 0; j <  cols(); ++j) {
            cv::Vec3f BGR(MAX_COLOR_LEVEL - blue(i, j), MAX_COLOR_LEVEL - green(i, j), MAX_COLOR_LEVEL - red(i, j));
            output(i, j) = BGR; 
        }
    }

    return output;
}
