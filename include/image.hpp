#ifndef IMAGE_HPP
#define IMAGE_HPP 

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <filesystem>
#include <numeric>


namespace img {

	enum class Type { GRAYSCALE, RGB };
	enum Color { BLACK = 0, WHITE = 255 };
	enum { R = 2, G = 1, B = 0 };

	template <Type t = Type::RGB>
	class Image {
	private:
		std::string m_name;
		cv::Mat m_data;

	public:
		Image(const std::filesystem::path& path);
		Image(unsigned rows = 0, unsigned cols = 0);
		Image(const cv::Mat& data)
			: m_data(data)
		{}

		operator bool() const
		{
			return !m_data.empty();
		}

		unsigned rows() const
		{
			return m_data.rows;
		}

		unsigned cols() const
		{
			return m_data.cols;
		}

		void show() const
		{
			namedWindow("Display window", cv::WINDOW_AUTOSIZE);
			imshow("Display window", m_data);
		}

		void info() const
		{
			std::cout << "m_name: " << m_name << std::endl;
			std::cout << "size: " << m_data.rows << " x " << m_data.cols << std::endl;
		}

		double get_aspect_ratio() const
		{
			return cols()/(double)rows();
		}

		Image<t> resize(int width, int height) const
		{
			cv::Size size(width, height);
			cv::Mat dst;
			cv::resize(m_data, dst, size);
			return Image<t>(dst);
		}

		Image<t> resize(int width) const
		{
			return resize(width, width/get_aspect_ratio());
		}

		Image<t> resize(const Image& image) const
		{
			return resize(image.cols(),image.rows());
		}

		Image<t> copy() const
		{
			Image<t> output;
			output.m_data = m_data.clone();
			return output;
		}

		unsigned pixel_sum(unsigned i, unsigned j) const;

		std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>& operator()(unsigned i, unsigned j);
		const std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>& operator()(unsigned i, unsigned j) const;

		void save(const std::filesystem::path& path) const
		{
			imwrite(path.string(), m_data);
		}

		std::conditional_t<t==Type::RGB, cv::MatIterator_<cv::Vec3b>, cv::MatIterator_<unsigned char>> begin();
		std::conditional_t<t==Type::RGB, cv::MatIterator_<cv::Vec3b>, cv::MatIterator_<unsigned char>> end();

		std::conditional_t<t==Type::RGB, cv::MatConstIterator_<cv::Vec3b>, cv::MatConstIterator_<unsigned char>> cbegin() const;
		std::conditional_t<t==Type::RGB, cv::MatConstIterator_<cv::Vec3b>, cv::MatConstIterator_<unsigned char>> cend() const;

		Image<Type::GRAYSCALE> grayscale() const;
		Image<Type::GRAYSCALE> black_white() const;
	};

	template <>
	Image<Type::RGB>::Image(const std::filesystem::path& path)
		: m_name(path.filename()), m_data(cv::imread(path.string(), cv::IMREAD_COLOR))
	{
	}

	template <>
	Image<Type::GRAYSCALE>::Image(const std::filesystem::path& path)
		: m_name(path.filename()), m_data(cv::imread(path.string(), cv::IMREAD_GRAYSCALE))
	{
	}

	template <>
	Image<Type::RGB>::Image(unsigned rows, unsigned cols)
		: m_data(cv::Mat(rows, cols, CV_8UC3, cv::Scalar(Color::WHITE, Color::WHITE, Color::WHITE)))
	{
	}

	template <>
	Image<Type::GRAYSCALE>::Image(unsigned rows, unsigned cols)
		: m_data(cv::Mat(rows, cols, CV_8UC1, cv::Scalar(Color::WHITE)))
	{
	}

	template<>
	cv::MatIterator_<cv::Vec3b> Image<Type::RGB>::begin()
	{
		return m_data.begin<cv::Vec3b>();
	}

	template<>
	cv::MatIterator_<unsigned char> Image<Type::GRAYSCALE>::begin()
	{
		return m_data.begin<unsigned char>();
	}

	template<>
	cv::MatIterator_<cv::Vec3b> Image<Type::RGB>::end()
	{
		return m_data.end<cv::Vec3b>();
	}

	template<>
	cv::MatIterator_<unsigned char> Image<Type::GRAYSCALE>::end()
	{
		return m_data.end<unsigned char>();
	}

	template<>
	cv::MatConstIterator_<cv::Vec3b> Image<Type::RGB>::cbegin() const
	{
		return m_data.begin<cv::Vec3b>();
	}

	template<>
	cv::MatConstIterator_<unsigned char> Image<Type::GRAYSCALE>::cbegin() const
	{
		return m_data.begin<unsigned char>();
	}

	template<>
	cv::MatConstIterator_<cv::Vec3b> Image<Type::RGB>::cend() const
	{
		return m_data.end<cv::Vec3b>();
	}

	template<>
	cv::MatConstIterator_<unsigned char> Image<Type::GRAYSCALE>::cend() const
	{
		return m_data.end<unsigned char>();
	}


	template<>
	cv::Vec3b& Image<Type::RGB>::operator()(unsigned i, unsigned j)
	{
		return m_data.at<cv::Vec3b>(i,j);
	}

	template<>
	unsigned char& Image<Type::GRAYSCALE>::operator()(unsigned i, unsigned j)
	{
		return m_data.at<unsigned char>(i,j);
	}

	template<>
	const cv::Vec3b& Image<Type::RGB>::operator()(unsigned i, unsigned j) const
	{
		return m_data.at<cv::Vec3b>(i,j);
	}

	template<>
	const unsigned char& Image<Type::GRAYSCALE>::operator()(unsigned i, unsigned j) const
	{
		return m_data.at<unsigned char>(i,j);
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

		for (unsigned i = 0; i < rows(); i++) {
			for (unsigned j = 0; j < cols(); j++) {
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

		for (unsigned i = 0; i < rows(); i++) {
			for (unsigned j = 0; j < cols(); j++) {
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

}; // end of namespace img
#endif /* IMAGE_HPP */
