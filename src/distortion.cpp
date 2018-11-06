#include <iostream>
#include <vector>
#include <eigen3/Eigen/Eigenvalues>
#include "image.hpp"

Eigen::MatrixXd M(3,4);
Eigen::MatrixXd MP(3,4);

static void on_mouse_1(int event, int x, int y, int, void*)
{
	static int j = 0;
	if (j > 3) return;
	if (event == cv::EVENT_LBUTTONDOWN) {
		std::cout << y << "," << x << std::endl;
		M.col(j++) << y, x, 1;
	}
}

static void on_mouse_2(int event, int x, int y, int, void*)
{
	static int j = 0;
	if (j > 3) return;
	if (event == cv::EVENT_LBUTTONDOWN) {
		std::cout << y << "," << x << std::endl;
		MP.col(j++) << y, x, 1;
	}
}


cv::Vec3b f2(const img::Image<img::Type::RGB>& img, double xd, double yd)
{
	double r = 0, g= 0, b=0;
	int x = xd;
	int y = yd;

	int xr = x+1;
	int yr = y;
	if (xr >= 0 && xr < (int)img.rows() && yr >= 0 && yr < (int)img.cols()) {
		r += img(xr, yr)[img::R]*(xd-x);
		g += img(xr, yr)[img::G]*(xd-x);
		b += img(xr, yr)[img::B]*(xd-x);
	}

	int xu = x;
	int yu = y+1;
	if (xu >= 0 && xu < (int)img.rows() && yu >= 0 && yu < (int)img.cols()) {
		r += img(xu, yu)[img::R]*(yd-y);
		g += img(xu, yu)[img::G]*(yd-y);
		b += img(xu, yu)[img::B]*(yd-y);
	}


	int xdd = x+1;
	int ydd = y+1;
	if (xdd >= 0 && xdd < (int)img.rows() && ydd >= 0 && ydd < (int)img.cols()) {
		r += img(xdd, ydd)[img::R]*(xd-x)*(yd-y);
		g += img(xdd, ydd)[img::G]*(xd-x)*(yd-y);
		b += img(xdd, ydd)[img::B]*(xd-x)*(yd-y);
	}

	if (x >= 0 && x < (int)img.rows() && y >= 0 && y < (int)img.cols()) {
		r += img(x, y)[img::R]*(3-(xd-x)-(yd-y)-(xd-x)*(yd-y));
		g += img(x, y)[img::G]*(3-(xd-x)-(yd-y)-(xd-x)*(yd-y));
		b += img(x, y)[img::B]*(3-(xd-x)-(yd-y)-(xd-x)*(yd-y));
	}

	r /= 3.0;
	g /= 3.0;
	b /= 3.0;

	return cv::Vec3b(std::round(b), std::round(g), std::round(r));
}

cv::Vec3b f(const img::Image<img::Type::RGB>& img, double xd, double yd)
{
	double r = 0, g= 0, b=0;
	int x = xd;
	int y = yd;

	int xr = x+1;
	int yr = y;
	if (xr >= 0 && xr < (int)img.rows() && yr >= 0 && yr < (int)img.cols()) {
		r += img(xr, yr)[img::R]*(xd-x);
		g += img(xr, yr)[img::G]*(xd-x);
		b += img(xr, yr)[img::B]*(xd-x);
	}

	int xu = x;
	int yu = y+1;
	if (xu >= 0 && xu < (int)img.rows() && yu >= 0 && yu < (int)img.cols()) {
		r += img(xu, yu)[img::R]*(yd-y);
		g += img(xu, yu)[img::G]*(yd-y);
		b += img(xu, yu)[img::B]*(yd-y);
	}


	int xdd = x+1;
	int ydd = y+1;
	if (xdd >= 0 && xdd < (int)img.rows() && ydd >= 0 && ydd < (int)img.cols()) {
		r += img(xdd, ydd)[img::R]*(xd-x)*(yd-y);
		g += img(xdd, ydd)[img::G]*(xd-x)*(yd-y);
		b += img(xdd, ydd)[img::B]*(xd-x)*(yd-y);
	}

	if (x >= 0 && x < (int)img.rows() && y >= 0 && y < (int)img.cols()) {
		r += img(x, y)[img::R]*((1-(xd-x))*(1-(yd-y)));
		g += img(x, y)[img::G]*((1-(xd-x))*(1-(yd-y)));
		b += img(x, y)[img::B]*((1-(xd-x))*(1-(yd-y)));
	}

	r /= ((xd-x)+(yd-y)+(xd-x)*(yd-y)+(1-(xd-x))*(1-(yd-y)));
	g /= ((xd-x)+(yd-y)+(xd-x)*(yd-y)+(1-(xd-x))*(1-(yd-y)));
	b /= ((xd-x)+(yd-y)+(xd-x)*(yd-y)+(1-(xd-x))*(1-(yd-y)));

	return cv::Vec3b(std::round(b), std::round(g), std::round(r));
}

int main()
{
	img::Image<img::Type::RGB> image("images/blackboard.jpg");
	img::Image<img::Type::RGB> image_display = image;
	image_display.show();

	// resize image
	while (true) {
		char key = cv::waitKey(0); 
		if (key == '-') {
			image_display = image.resize(image_display.cols()-100);
			image_display.show();
		}
		else if (key == '+' || key == '=') {
			image_display = image.resize(image_display.cols()+100);
			image_display.show();
		}
		else if (key == 27) break;
	}

	std::cout << "choose 4 points:" << std::endl;

	cv::setMouseCallback("Display window", on_mouse_1);

	while (true) {
		char key = cv::waitKey(0);
		if (key == 27) break;
	}

	double coeff = image.rows()/(double)image_display.rows();
	M.block(0, 0, 2, 4) *= coeff;

	img::Image<img::Type::RGB> output(image.rows(), image.cols());
	output.resize(image_display).show();
	cv::setMouseCallback("Display window",on_mouse_2);
	while (true) {
		char key = cv::waitKey(0);
		if (key == 27) break;
	}

	MP.block(0, 0, 2, 4) *= coeff;

	MP.col(1)[0] = MP.col(0)[0];
	MP.col(1)[1] = MP.col(2)[1];
	MP.col(3)[0] = MP.col(2)[0];
	MP.col(3)[1] = MP.col(0)[1];


	Eigen::Vector3d lambda = M.block(0, 0, 3, 3).inverse()*M.col(3);
	Eigen::MatrixXd P1(3,3);
	P1.col(0) = lambda(0)*M.col(0);
	P1.col(1) = lambda(1)*M.col(1);
	P1.col(2) = lambda(2)*M.col(2);

	Eigen::Vector3d lambdap = MP.block(0, 0, 3, 3).inverse()*MP.col(3);
	Eigen::MatrixXd P2(3,3);
	P2.col(0) = lambdap(0)*MP.col(0);
	P2.col(1) = lambdap(1)*MP.col(1); P2.col(2) = lambdap(2)*MP.col(2);

	Eigen::Matrix3d P = P2*P1.inverse();
	Eigen::Matrix3d PI = P.inverse();

	for (unsigned i = 0; i < output.rows(); i++) {
		for (unsigned j = 0; j < output.cols(); j++) {
			Eigen::Vector3d w = PI*Eigen::Vector3d{1.0*i, 1.0*j, 1.0};

			double x = (w(0)/w(2));
			double y = (w(1)/w(2));
			output(i, j) = f(image,x,y);
		}
	}

	output.resize(image_display).show();
	while (true) {
		char key = cv::waitKey(0);
		if (key == 27) break;
	}

	output.save("out.png");

	return 0;
}
