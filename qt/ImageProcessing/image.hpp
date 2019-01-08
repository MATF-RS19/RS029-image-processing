#ifndef IMAGE_HPP
#define IMAGE_HPP 

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <experimental/filesystem>
#include <numeric>
#include <string>
#include <thread>
#include <future>


namespace img {

	template <typename Function, typename... Args>
	void start_threads(int start, int end, const Function& func, Args&&... args)
	{
		int num_threads = std::thread::hardware_concurrency();
		std::vector<std::future<void>> threads(num_threads);
		int rows = end;
		for (int i = 0; i < num_threads; i++) {
			int from = std::max(rows/num_threads * i, start);
			int to = (i==num_threads-1) ? rows : from + rows/num_threads;
			// caution - from and to mustn't be captured with &
			threads[i] = std::async([&,from,to]{ std::invoke(func, std::forward<Args>(args)..., from, to); });
		}
	}

	enum class Type { GRAYSCALE, RGB };
	enum Color { BLACK = 0, WHITE = 255 };
	enum { R = 2, G = 1, B = 0 };

	template <Type t = Type::RGB>
	class Image {
	private:
		std::string m_name;
		cv::Mat m_data;

		void gaussian_blur_help(Image<Type::GRAYSCALE>& output, const std::vector<float>& gauss, int from, int to) const;

	public:
		Image(const std::experimental::filesystem::path& path);
		Image(unsigned rows = 0, unsigned cols = 0, Color c = Color::WHITE, std::string name = "");
		Image(const cv::Mat& data)
			: m_name(), m_data(data)
		{}

		Image(cv::Mat&& data)
			: m_name(), m_data(std::move(data))
		{}

		Image(const Image& other)
			: m_name(other.m_name), m_data(other.m_data.clone())
		{}

		// we need this because we have defined copy constructor
		Image(Image&& other) = default;
		Image& operator=(const Image& other) = default;
		Image& operator=(Image&& other) = default;

		std::string name() const
		{
			return m_name;
		}

		std::string purename() const
		{
			return std::string(m_name.cbegin(), std::find(m_name.cbegin(), m_name.cend(), '.'));
		}

		void set_name(std::string name)
		{
			m_name = std::move(name);
		}

		unsigned pixels() const
		{
			return cols()*rows();
		}

		operator bool() const
		{
			return !m_data.empty();
		}

		void bgr2rgb();

		void set_borders(int thickness = 1, Color c = img::BLACK)
		{
			for (int i = 0; i < rows(); ++i) {
				for (int j = 0; j < thickness; ++j) {
					(*this)(i, j) = (*this)(i, cols()-1-j) = c;
				}
			}

			for (int j = 0; j < cols(); j++) {
				for (int i = 0; i < thickness; i++) {
					(*this)(i, j) = (*this)(rows()-1-i, j) = c;
				}
			}
		}

		void set_borders(int thickness, const Image<t>& other)
		{
			if (dimension() != other.dimension()) return;

			for (int i = 0; i < rows(); ++i) {
				for (int j = 0; j < thickness; ++j) {
					(*this)(i, j) = other(i,j);
					(*this)(i, cols()-1-j) = other(i, cols()-1-j);
				}
			}

			for (int j = 0; j < cols(); j++) {
				for (int i = 0; i < thickness; i++) {
					(*this)(i, j) = other(i, j);
					(*this)(rows()-1-i, j) = other(rows()-1-i,j);
				}
			}
		}

		uchar* data() const
		{
			return m_data.data;
		}

		img::Type type() const
		{
			return t;
		}

		int step() const
		{
			return m_data.step;
		}

		bool in_range(int x, int y) const
		{
			return x >= 0 && x < rows() && y >= 0 && y < cols();
		}

		std::pair<int, int> dimension() const
		{
			return {rows(), cols()};
		}

		int rows() const
		{
			return m_data.rows;
		}

		int cols() const
		{
			return m_data.cols;
		}


		void save(const std::experimental::filesystem::path& path) const
		{
			if (!path.empty() && !m_data.empty())
				imwrite(path.string(), m_data);
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
			cv::Mat dst;
			cv::resize(m_data, dst, cv::Size(width, height), 0, 0, cv::INTER_AREA);
			return Image<t>(std::move(dst));
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

		// 5x5 gaussian filter with standard deviation 1.4
		Image<Type::GRAYSCALE> gaussian_blur() const;

		Image<Type::GRAYSCALE> grayscale() const;
		Image<Type::GRAYSCALE> black_white() const;

		std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>& operator()(unsigned i, unsigned j)
		{
			return m_data.at<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>(i,j);
		}

		const std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>& operator()(unsigned i, unsigned j) const
		{
			return m_data.at<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>(i,j);
		}

		std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>& operator()(const std::pair<unsigned, unsigned>& p)
		{
			return m_data.at<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>(p.first, p.second);
		}

		const std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>& operator()(const std::pair<unsigned, unsigned>& p) const 
		{
			return m_data.at<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>(p.first, p.second);
		}


		std::conditional_t<t==Type::RGB, cv::MatIterator_<cv::Vec3b>, cv::MatIterator_<unsigned char>> begin()
		{
			return m_data.begin<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>();
		}

		std::conditional_t<t==Type::RGB, cv::MatConstIterator_<cv::Vec3b>, cv::MatConstIterator_<unsigned char>> begin() const
		{
			return m_data.begin<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>();
		}

		std::conditional_t<t==Type::RGB, cv::MatConstIterator_<cv::Vec3b>, cv::MatConstIterator_<unsigned char>> cbegin() const
		{
			return m_data.begin<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>();
		}


		std::conditional_t<t==Type::RGB, cv::MatIterator_<cv::Vec3b>, cv::MatIterator_<unsigned char>> end()
		{
			return m_data.end<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>();
		}

		std::conditional_t<t==Type::RGB, cv::MatConstIterator_<cv::Vec3b>, cv::MatConstIterator_<unsigned char>> end() const
		{
			return m_data.end<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>();
		}

		std::conditional_t<t==Type::RGB, cv::MatConstIterator_<cv::Vec3b>, cv::MatConstIterator_<unsigned char>> cend() const
		{
			return m_data.end<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>();
		}

		std::conditional_t<t==Type::RGB, cv::MatIterator_<cv::Vec3b>, cv::MatIterator_<unsigned char>> operator[](unsigned i)
		{
			return m_data.begin<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>() + i*cols();
		}

		std::conditional_t<t==Type::RGB, cv::MatConstIterator_<cv::Vec3b>, cv::MatConstIterator_<unsigned char>> operator[](unsigned i) const
		{
			return m_data.begin<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>() + i*cols();
		}
	}; // end of class image

}; // end of namespace img
#endif /* IMAGE_HPP */
