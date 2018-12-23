#include "image.hpp"
using namespace img;

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

template <>
cv::MatIterator_<cv::Vec3b> Image<Type::RGB>::operator[](unsigned i)
{
    return m_data.begin<cv::Vec3b>() + i*cols();
}

template <>
cv::MatIterator_<unsigned char> Image<Type::GRAYSCALE>::operator[](unsigned i)
{
    return m_data.begin<unsigned char>() + i*cols();
}

template<>
cv::MatConstIterator_<cv::Vec3b> Image<Type::RGB>::operator[](unsigned i) const
{
    return m_data.begin<cv::Vec3b>() + i*cols();
}

template<>
cv::MatConstIterator_<unsigned char> Image<Type::GRAYSCALE>::operator[](unsigned i) const
{
    return m_data.begin<unsigned char>() + i*cols();
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
cv::Vec3b& Image<Type::RGB>::operator()(const std::pair<unsigned, unsigned>& p)
{
    return m_data.at<cv::Vec3b>(p.first, p.second);
}

template<>
unsigned char& Image<Type::GRAYSCALE>::operator()(const std::pair<unsigned, unsigned>& p)
{
    return m_data.at<unsigned char>(p.first, p.second);
}

template<>
const cv::Vec3b& Image<Type::RGB>::operator()(const std::pair<unsigned, unsigned>& p) const
{
    return m_data.at<cv::Vec3b>(p.first, p.second);
}

template<>
const unsigned char& Image<Type::GRAYSCALE>::operator()(const std::pair<unsigned, unsigned>& p) const
{
    return m_data.at<unsigned char>(p.first, p.second);
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
