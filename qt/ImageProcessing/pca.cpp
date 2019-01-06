#include "pca.hpp"
#include <eigen3/Eigen/Eigenvalues>
#include <range/v3/view.hpp>
#include <range/v3/action/sort.hpp>


std::vector<double> vector(const Eigen::VectorXd& v)
{
	return {v.data(), v.data() + v.rows() * v.cols()};
}

img::Image<img::Type::RGB> pca(const img::Image<img::Type::RGB>& img, int k)
{
	int n, d;
	bool transpose = false;

	if (img.rows() <= img.cols()) {
		d = img.rows(); // dimenstions
		n = img.cols();
	}
	else {
		d = img.cols(); // dimenstions
		n = img.rows();
		transpose = true;
	}

	k = std::min(k, d); // cannot take more than number of dimensions

	img::Image<img::Type::RGB> compressed(img.rows(), img.cols());


	for (int c = 0; c < 3; c++) {
		Eigen::MatrixXd A(d, n);

		for (int i = 0; i < d; i++) {
			for (int j = 0; j < n; j++) {
				if (transpose)
					A(i, j) = img(j,i)[c];
				else
					A(i, j) = img(i,j)[c];
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


		for (int i = 0; i < compressed.rows(); i++) {
			for (int j = 0; j < compressed.cols(); j++) {
				if (transpose)
					compressed(i,j)[c] = (Reconstruction(j, i) < 0) ? 0 : Reconstruction(j,i);
				else
					compressed(i,j)[c] = (Reconstruction(i,j) < 0) ? 0 : Reconstruction(i,j);
			}
		}
	}

	return compressed;
}
