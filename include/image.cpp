#include "image.hpp"
using namespace img;

#define MAX_COLOR_LEVEL 255.0
#define HALF_CIRCLE     180.0
#define MAX_BINS        256

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

template<>
Image<Type::GRAYSCALE> Image<Type::GRAYSCALE>::negative() const
{
    // Samarth Jain, Megha Soni
    // "Digital Image Quality Enhancement Using Compound Contrast Enhancement Process"
    // IJEDR, Volume 6, Issue 3
    // https://www.ijedr.org/papers/IJEDR1803071.pdf

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

template<>
Image<Type::RGB> Image<Type::RGB>::ycrcb2rgb() const
{
    // formula source: https://www.w3.org/Graphics/JPEG/jfif3.pdf

    Image<Type::RGB> output(rows(), cols());

    for (int i = 0; i < rows(); ++i) {
            for (int j = 0; j < cols(); ++j) {
                    int R = std::round(
                            red(i, j) 
                            + 1.402 * (green(i, j) - 128) 
                            );
                    int G = std::round(
                            red(i, j)
                            - 0.34414 * (blue(i, j) - 128)
                            - 0.71414 * (green(i, j) - 128)
                            );  
                    int B = std::round(
                            red(i, j) + 1.772 * (blue(i, j) - 128)
                            );
                    cv::Vec3f RGB(B, G, R);
                    output(i,j) = RGB;
            }
    }

    return output;
}

template<>
Image<Type::RGB> Image<Type::RGB>::rgb2ycrcb() const
{
    // formula source: https://www.w3.org/Graphics/JPEG/jfif3.pdf 

    Image<Type::RGB> output(rows(), cols());

    for (int i = 0; i < rows(); ++i) {
            for (int j = 0; j < cols(); ++j) {
                    int Y = std::round(
                            0.299 * red(i, j) 
                            + 0.587 * green(i, j) 
                            + 0.114 * blue(i, j)
                            );
                    int Cb = std::round(
                            128 - 0.1687 * red(i, j)
                            - 0.3313 * green(i, j)
                            + 0.5 * blue(i, j)
                            );  
                    int Cr = std::round(
                            128 + 0.5 * red(i, j)
                            - 0.4187 * green(i, j)
                            - 0.0813 * blue(i, j)
                        );
                    cv::Vec3f yCrCbPixel(Y, Cr, Cb);
                    output(i,j) = yCrCbPixel;

            }
    }

    return output;
}

template<>
Image<Type::RGB> Image<Type::RGB>::rgb2hsi() const
{
    Image<Type::RGB> output(rows(), cols());

    for (int i = 0; i < rows(); ++i) {
            for (int j = 0; j < cols(); ++j) {
                std::vector<unsigned> RGB {red(i, j), green(i, j), blue(i, j)};
                unsigned min_RGB = std::distance(RGB.cbegin(), std::min_element(RGB.cbegin(), RGB.cend()));
                unsigned R_B = red(i, j) - blue(i, j);
                unsigned R_G = red(i, j) - green(i, j);
                unsigned H, S, I;

                unsigned sum_pixel = pixel_sum(i, j);

                H = 1 / ((0.5 * R_G + R_B) / sqrt(R_G * R_G + R_B * R_G));
                S = 1 - 3 * min_RGB / sum_pixel;
                I = sum_pixel / 3;

                cv::Vec3f HSIPixel(H, S, I);
                output(i,j) = HSIPixel;
            }
    }

    return output;
}

template<>
Image<Type::RGB> Image<Type::RGB>::rgb2hsb() const
{
    Image<Type::RGB> output(rows(), cols());

    for (int i = 0; i < rows(); ++i) {
            for (int j = 0; j < cols(); ++j) {
                std::vector<unsigned> RGB {red(i, j), green(i, j), blue(i, j)};
                unsigned max_RGB = std::distance(RGB.cbegin(), std::max_element(RGB.cbegin(), RGB.cend()));
                unsigned min_RGB = std::distance(RGB.cbegin(), std::min_element(RGB.cbegin(), RGB.cend()));
                int delta = RGB[max_RGB] - RGB[min_RGB];
                double H, S, B;
                
                if (delta < 0) {
                    H = 0;
                    S = 0;
                    B = 0;
                }
                else {
                    double R1 = (RGB[max_RGB] - red(i, j)) / delta;
                    double G1 = (RGB[max_RGB] - green(i, j)) / delta;
                    double B1 = (RGB[max_RGB] - blue(i, j)) / delta;

                    if (max_RGB == 0) {
                        H = B1 - G1;  
                    }
                    else if (max_RGB == 1) {
                        H = R1 - B1 + 2.0;
                    }
                    else {
                            H = G1 - R1 + 4.0;
                    }
                    if (H < 0) {
                            H = H / 6 + 1.0;
                    }
                    S = RGB[max_RGB] / delta;
                    B = RGB[max_RGB] / MAX_COLOR_LEVEL;
                    cv::Vec3f HSBPixel(H, S, B);
                    output(i,j) = HSBPixel;
                }
            }
    }

    return output;
}

template<>
Image<Type::RGB> Image<Type::RGB>::rgb2hsv() const
{
    Image<Type::RGB> output(rows(), cols());

    for (int i = 0; i < rows(); ++i) {
        for (int j = 0; j < cols(); ++j) {
            // scale to [0, 1]
            double R = red(i, j) / MAX_COLOR_LEVEL;
            double G = green(i, j) / MAX_COLOR_LEVEL;
            double B = blue(i, j) / MAX_COLOR_LEVEL;
            
            std::vector<double> RGB { R, G, B };

            double max_RGB = std::distance(RGB.cbegin(), std::max_element(RGB.cbegin(), RGB.cend()));
            double min_RGB = std::distance(RGB.cbegin(), std::min_element(RGB.cbegin(), RGB.cend()));
            double delta = RGB[max_RGB] - RGB[min_RGB];
            double H, S, V;

            V = RGB[max_RGB];

            if (RGB[max_RGB] <= 0 or delta <= 0.0001) {
                S = 0;
                H = 0;
            }
            else {
                S = delta / RGB[max_RGB];
                // maximum is red
                if (max_RGB == 0) {
                    H = 60.0 * (G - B) / delta;
                }
                // maximum is green
                else if (max_RGB == 1) {
                    H = 120.0 + 60.0 * (B - R) / delta;
                }
                // maximum is blue
                else {
                    H = 240.0 + 60.0 * (R - G) / delta;
                }

                if (H < 0) H += 360;
            }

            cv::Vec3f HSVPixel(H, S * 100, V * 100);
            output(i,j) = HSVPixel;
        }
    }

    return output;
}

template<>
Image<Type::RGB> Image<Type::RGB>::hsv2rgb() const
{
    Image<Type::RGB> output(rows(), cols());

    for (int i = 0; i < rows(); ++i) {
        for (int j = 0; j < cols(); ++j) {
            double H = (*this)(i, j)[0];
            double S = (*this)(i, j)[1] / 100.0;
            double V = (*this)(i, j)[2] / 100.0;
            
            double R = 0;
            double G = 0;
            double B = 0;
            
            if (S <= 0) {
                R = G = B = V;
            }
            else {
                if (H >= 360) H = 0;
                H /= 60;    // H e [0, 5]
                unsigned i = floor(H); 
                double f = H - i;
                double p = V * (1.0 - S);
                double q = V * (1.0 - S * f);
                double t = V * (1.0 - S * (1.0 - f));
                
                switch (i) {
                    case 0:
                        R = V;
                        G = t;
                        B = p;
                        break;
                    case 1:
                        R = q;
                        G = V;
                        B = p;
                        break;
                    case 2:
                        R = p;
                        G = V;
                        B = t;
                        break;
                    case 3:
                        R = p;
                        G = q;
                        B = V;
                        break;
                    case 4:
                        R = t;
                        G = p;
                        B = V;
                        break;
                    case 5:
                        R = V;
                        G = p;
                        B = q;
                        break;
                }
            }

            cv::Vec3f RGBPixel(B * MAX_COLOR_LEVEL, G * MAX_COLOR_LEVEL, R * MAX_COLOR_LEVEL);
            output(i,j) = RGBPixel;  
        }
    }

    return output;
}

template<>
Image<Type::RGB> Image<Type::RGB>::color_complement() const
{
    Image<Type::RGB> output(rows(), cols());
    output = rgb2hsv();

    for (int i = 0; i < rows(); ++i) {
        for (int j = 0; j < cols(); ++j) {
            double H = output.blue(i,j);
            H < HALF_CIRCLE ? H += HALF_CIRCLE : H -= HALF_CIRCLE;
            cv::Vec3f HSVPixel(H, output.green(i,j), output.red(i,j)); 
            output(i, j) = HSVPixel;
        }
    }

    return output.hsv2rgb();
}

template<>
std::vector<unsigned> Image<Type::GRAYSCALE>::histogram() const
{
    std::vector<unsigned> histogram(MAX_BINS, 0);

    for (int i = 0; i < rows(); i++) {
        for (int j = 0; j <  cols(); ++j) {
            histogram[(*this)(i, j)]++;
        }
    }

    return histogram;
}

template<>
std::vector<std::tuple<unsigned, unsigned, unsigned>> Image<Type::RGB>::channels_histogram() const
{
    std::vector<std::tuple<unsigned, unsigned, unsigned>> histogram;
    histogram.reserve(MAX_BINS);

    std::vector<unsigned> R_histogram(MAX_BINS, 0);
    std::vector<unsigned> G_histogram(MAX_BINS, 0);
    std::vector<unsigned> B_histogram(MAX_BINS, 0);
    
    for (int i = 0; i < rows(); ++i) {
        for (int j = 0; j < cols(); ++j) {
            R_histogram[red(i, j)]++;
            G_histogram[green(i, j)]++;
            B_histogram[blue(i, j)]++;
        }
    }

    for (int i = 0; i < MAX_BINS; ++i) {
        histogram.emplace_back(std::make_tuple( R_histogram[i], 
                                                G_histogram[i], 
                                                B_histogram[i] ));
    }

    return histogram;
}
