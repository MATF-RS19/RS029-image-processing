#include "fuzzy_edge_detection.hpp"
#include <iostream>
#include <cmath>
#include <thread>
#include <mutex>

// class implements fuzzy edge detection alorithm
class fed {
public:
	fed(const img::Image<img::Type::GRAYSCALE>& img)
		: m_img(img),
		m_mi(img.rows(), std::vector<unsigned>(img.cols(), 0))
	{}

	const img::Image<img::Type::GRAYSCALE>& detect(float threshold)
	{
		m_output = m_img.gaussian_blur();
		defuzzification(update()*threshold);
		return m_output;
	}

private:
	// input image
	img::Image<img::Type::GRAYSCALE> m_output;
	// resulting image
	const img::Image<img::Type::GRAYSCALE>& m_img;
	// characteristic function
	std::vector<std::vector<unsigned>> m_mi;
	// shades of gray
	const int L = 256;
	std::mutex m_mutex;

	void update_help(unsigned& maxm, int from, int to)
	{
		unsigned maxm_tmp = 0;
		for (int i = from; i < to; i++) {
			for (int j = 1; j < m_output.cols()-1; j++) {
				unsigned s =0;
				for (int x = -1; x <= 1; x++) {
					for (int y = -1; y <= 1; y++) {
						s += std::abs(m_output(i,j)-m_output(i+x,j+y));
					}
				}
				m_mi[i][j] = 1000*s/(s+(L-1));
				maxm_tmp = std::max(maxm_tmp, m_mi[i][j]);
			}
		}

		std::lock_guard<std::mutex> lock(m_mutex);
		maxm = std::max(maxm, maxm_tmp);
	}

	// calculate characteristic function and return its maximum value
	unsigned update()
	{
		unsigned maxm = 0;
		img::start_threads(1, m_output.rows()-1, &fed::update_help, this, maxm);
		return maxm;
	}

	void defuzzification_help(unsigned limit, int from, int to)
	{
		for (int i = from; i < to; ++i) {
			for (int j = 1; j < m_output.cols()-1; j++) {
				// img(i,j)=(L-1)*mi[i][j]/1000;
				m_output(i,j) = (m_mi[i][j] < limit) ? img::BLACK : img::WHITE;
			}
		}
	}

	void defuzzification(unsigned limit)
	{
		img::start_threads(1, m_output.rows()-1, &fed::defuzzification_help, this, limit);
	}
};


img::Image<img::Type::GRAYSCALE> fuzzy_edge_detection(const img::Image<img::Type::GRAYSCALE>& img, float threshold)
{
	return fed(img).detect(threshold);
}

