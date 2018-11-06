#include "image.hpp"
#include <eigen3/Eigen/Eigenvalues>
#include <range/v3/view.hpp>
#include <range/v3/action/sort.hpp>


std::vector<double> vector(const Eigen::VectorXd& v)
{
	return {v.data(), v.data() + v.rows() * v.cols()};
}

int main()
{
	img::Image<img::Type::RGB> img("images/cat.jpg");

	int n = 600;
	int d = 398;

	img::Image<img::Type::RGB> compressed(d, n);

	int k;
	std::cout << "k=";
	std::cin >> k;

	for (int c = 0; c < 3; c++) {
		Eigen::MatrixXd A(d, n);

		for (int i = 0; i < d; i++) {
			for (int j = 0; j < n; j++) {
				A(i,j) = img(i,j)[c];
			}
		}

		Eigen::VectorXd mean_vec = A.rowwise().mean();

		A =  A.colwise()-mean_vec;

		auto cov_mat = A*A.transpose() / (n-1);


		Eigen::EigenSolver<Eigen::MatrixXd> s(cov_mat);

		std::vector<double> eigen_values = vector(s.eigenvalues().real());
		auto sorted_eigen_values = ranges::view::zip(eigen_values, ranges::view::ints(0)) 
									| ranges::v3::to_vector
									| ranges::action::sort(std::greater<std::pair<double, int>>{});
		auto eigen_indices = sorted_eigen_values
					| ranges::view::transform([] (auto&& value) { return value.second; })
					| ranges::view::take(k);


		const Eigen::MatrixXd& eigen_vectors = s.eigenvectors().real();
		Eigen::MatrixXd P(k, d);
		for (int i = 0; i < k; i++) {
			P.row(i) = eigen_vectors.col(eigen_indices[i]);
		}

		Eigen::MatrixXd Projected = P*A;
		Eigen::MatrixXd Reconstruction = (P.transpose()*Projected).colwise() + mean_vec;


		for (int i = 0; i < d; i++) {
			for (int j = 0; j < n; j++) {
				compressed(i,j)[c] = (Reconstruction(i,j) < 0) ? 0 : Reconstruction(i,j);
			}
		}
	}

	compressed.show();
	cv::waitKey(0);
	compressed.save("pca_output.png");

    return 0;
}
