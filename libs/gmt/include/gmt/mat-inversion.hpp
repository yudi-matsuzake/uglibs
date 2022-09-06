#pragma once

#include <ranges>
#include <cmath>
#include <assert.h>
#include <stdexcept>

#include <list>

#include "gmt/mat.hpp"

namespace gmt {

template<typename T, uint64_t Rows, uint64_t Cols>
class system_has_no_solution : public std::runtime_error {
public:
	system_has_no_solution(const mat<T, Rows, Cols>& A, const mat<T, Rows, 1> B)
		: std::runtime_error(
			"linear algebra error:"
			" system has no solution"),
		  A(A), B(B)
	{}

	mat<T, Rows, Cols> A;
	mat<T, Rows, 1> B;
};

namespace detail{

template<class T>
constexpr auto swap_rows(T& a, auto i, auto j)
{
	swap(a[i], a[j]);
}

template<typename T, uint64_t Rows, uint64_t Cols>
constexpr auto row_operation(
	mat<T, Rows, Cols>& m,
	auto a,
	auto b,
	auto alpha,
	auto beta,
	auto first_column)
{
	for(auto i=first_column; i<Cols; ++i)
		m[a][i] = alpha*m[a][i] + beta*m[b][i];
}

template<typename T, uint64_t Rows, uint64_t Cols, class SwapOp, class RowOp>
constexpr
void gaussian_elimination(
	mat<T, Rows, Cols>& a,
	SwapOp&& swap_op,
	RowOp&& row_op)
{
	static_assert(Rows > 1 && Cols > 1);

	for(uint64_t i=0; i<a.n_row; i++){

		/*
		 * search for the maximal value in this column
		 */
		T max = std::abs(a[i][i]);
		uint64_t max_row = i;

		for(uint64_t j=i+1; j< a.n_row; ++j){
			T tmp = std::abs(a[j][i]);
			if(tmp > max){
				max_row = j;
				max = tmp;
			}
		}

		/*
		 * no pivot in this column
		 */
		if(a[max_row][i] == 0.0)
			continue;

		/*
		 * swap the maximum row with the current
		 */
		if(i != max_row){
			swap_rows(a, i, max_row);
			swap_op(i, max_row);
		}

		/*
		 * make element in the diagonal equal to 1
		 */
		auto const coef = 1./a[i][i];
		a[i][i] = T{1};
		row_operation(a, i, i, T{0}, coef, i+1);
		row_op(i, i, T{0}, coef);

		/*
		 * Make all cells below the diagonal equal to zero in the `i`-th column
		 */
		for(size_t j = i+1; j< a.n_row; ++j){
			auto const coef = a[j][i];
			a[j][i] = T{0};
			row_operation(a, j, i, T{1}, -coef, i+1);
			row_op(j, i, T{1}, -coef);
		}

	}

	/*
	 * make all elements above diagonal equal to zero
	 */
	for(auto i=1UL; i<a.n_row; ++i){
		auto const c_idx = a.n_row - i;

		for(auto j=i+1UL; j<=a.n_row; ++j){
			auto const r_idx = a.n_row - j;
			auto const coef = a[r_idx][c_idx];
			a[r_idx][c_idx] = T{0};

			if(coef != 0.0){
				row_operation(a, r_idx, c_idx, T{1}, -coef, c_idx+1UL);
				row_op(r_idx, c_idx, T{1}, -coef);
			}
		}
	}
}

} // end of namespace detail

/**
 * resolves the system `a`, no format A*x = b
 */
template<typename T, uint64_t Rows, uint64_t Cols>
constexpr auto resolve(
	mat<T, Rows, Cols>& a,
	mat<T, Rows, 1>& b)
{
	detail::gaussian_elimination(
		a, 
		[&b](auto i, auto j){ detail::swap_rows(b, i, j); },
		[&b](auto r, auto c, auto alpha, auto beta){
			detail::row_operation(b, r, c, alpha, beta, 0UL);
		}
	);
}

};
