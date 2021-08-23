#pragma once

#include "gmt/mat.hpp"
#include "gmt/vector.hpp"
#include <stdexcept>

namespace gmt{

template<class T, uint64_t N>
T parallelotope_signed_area(std::array<vector<T, N>, N> const& a)
{
	mat<T, N, N> m;

	for(uint64_t i=0; i<m.n_row; ++i)
		for(uint64_t j=0; j<m.n_col; ++j)
			m[i][j] = a[i][j];

	return determinant(m);
}

/**
  * in \reals^n, computes an orthogonal vector v\in\reals^n of N-1
  * given vectors
  *
  * this functions is called cross product because it is an multilinear
  * generalization of the cross product
  */
template<class T, uint64_t N>
vector<T, N> cross_product(std::array<vector<T, N>, N-1> const& a)
{
	vector<T, N> product;

	mat<T, N-1, N> m;

	for(uint64_t i=0; i<m.n_row; ++i)
		for(uint64_t j=0; j<m.n_col; ++j)
			m[i][j] = a[i][j];

	mat<T, N-1, N-1> sub_matrix;

	auto sign = T{1};

	for(uint64_t i=0; i<N; i++){
		uint64_t subidx = 0;

		for(uint64_t j=0; j<m.n_row; j++){
			for(uint64_t k=0; k<m.n_col; k++){
				if(k != i){
					uint64_t subi = subidx/sub_matrix.n_row;
					uint64_t subj = subidx%sub_matrix.n_col;

					sub_matrix[subi][subj] = m[j][k];
					subidx++;
				}
			}
		}

		product[i] = sign*determinant(sub_matrix);
		sign *= T{-1};

	}

	return product;
}

template<class T>
vector<T, 2> compute_orthogonal_vector(vector<T, 2> const& v)
{
	if(v[0] == 0.0 && v[1] == 0.0){
		throw std::runtime_error(
			"cannot compute orthogonal vector of all zeros"
		);
	}

	return v[0] > 0
		? vector<T, 2>{-v[1]/v[0], 1.}
		: vector<T, 2>{1., -v[0]/v[1]};
}

template<class T>
vector<T, 2> compute_orthogonal_vector(std::array<vector<T, 2>, 1> const& a)
{
	return compute_orthogonal_vector(a[0]);
}

template<class T, uint64_t N>
vector<T, N> compute_orthogonal_vector(std::array<vector<T, N>, N-1> const& a)
{
	return cross_product(a);
}

} // end of namespace gmt
