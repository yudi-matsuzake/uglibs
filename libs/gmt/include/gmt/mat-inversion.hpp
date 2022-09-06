#pragma once

#include <cmath>
#include <assert.h>
#include <stdexcept>

#include <list>

#include "gmt/mat.hpp"

namespace gmt {

template<typename T, uint64_t l, uint64_t c>
class system_has_no_solution : public std::runtime_error {
public:
	system_has_no_solution(const mat<T, l, c>& A, const mat<T, l, 1> B)
		: std::runtime_error(
			"linear algebra error:"
			" system has no solution"),
		  A(A), B(B)
	{}

	mat<T, l, c> A;
	mat<T, l, 1> B;
};

template<class T>
constexpr auto swap_rows(T& a, auto i, auto j)
{
	swap(a[i], a[j]);
}

template<typename T, uint64_t l, uint64_t c>
constexpr
mat<T, l, 1> gaussian_elimination(mat<T, l, c> const& a, mat<T, l, 1> const& b)
{
	static_assert(l > 1 && c > 1);

	mat<T, l, c> A = a;
	mat<T, l, 1> B = b;
	mat<T, l, 1> x;

	for(uint64_t i=0; i<A.n_row; i++){

		/*
		 * search for the maximal value in this column
		 */
		T max = std::abs(A[i][i]);
		uint64_t max_row = i;

		for(uint64_t j=i+1; j< A.n_row; j++){
			T tmp = std::abs(A[j][i]);
			if(tmp > max){
				max_row = j;
				max = tmp;
			}
		}

		/*
		 * if A[max_row][i] was equal to zero, the system might
		 * not have a solution
		 */
		if(A[max_row][i] == 0){
			if(max_row <= (A.n_col-1))
				throw system_has_no_solution<T, l, c>(a, b);
			else
				break; // overdetermined
		}

		/*
		 * swap the maximum row with the current
		 */
		if(i != max_row){
			swap_rows(A, i, max_row);
			swap_rows(B, i, max_row);
		}

		/*
		 * Make all rows below this zero in the current column
		 */
		for(size_t j = i+1; j< A.n_row; j++){
			T coef = A[j][i]/A[i][i];

			for(size_t k=i; k<A.n_col; k++){
				if(i == k)
					A[j][k] = 0;
				else
					A[j][k] -= coef*A[i][k];
			}

			B[j][0] -= coef*B[i][0];
		}

	}

	/*
	 * resolve x with the diagonal
	 */
	size_t i = A.n_row;
	do{
		i--;
		x[i][0] = B[i][0]/A[i][i];
		size_t j = i;

		if(j != 0){
			do{
				j--;
				B[j][0] -= A[j][i] * x[i][0];
			}while(j != 0);
		}
	}while(i != 0);

	return x;
}

/**
 * resolves the system `a`, no format A*x = b
 */
template<typename T, uint64_t l, uint64_t c>
constexpr mat<T, l, 1> resolve(const mat<T, l, c>& a, const mat<T, l, 1>& b)
{
	return gaussian_elimination(a, b);
}

};
