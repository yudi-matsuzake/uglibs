#pragma once

#include <cmath>
#include <ranges>
#include <optional>

#include <list>

#include "gmt/mat.hpp"

namespace gmt {

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

	constexpr auto m = std::min(a.n_row, a.n_col);
	for(uint64_t i=0UL; i<m; i++){

		/*
		 * search for the maximal value in this column
		 */
		auto pivot = [&]{
			for(auto j=i; j<a.n_row; ++j)
				if(a[j][i] != T{0})
					return j;
			return a.n_row;
		}();

		/*
		 * no pivot in this column
		 */
		if(pivot == a.n_row)
			continue;

		/*
		 * swap the maximum row with the current
		 */
		if(i != pivot){
			swap_rows(a, i, pivot);
			swap_op(i, pivot);
		}

		/*
		 * make element in the diagonal equal to 1
		 */
		if(a[i][i] != T{1}){
			auto const coef = 1./a[i][i];
			a[i][i] = T{1};
			row_operation(a, i, i, T{0}, coef, i+1);
			row_op(i, i, T{0}, coef);
		}

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
	for(auto i=1UL; i<m; ++i){
		auto const c_idx = m - i;

		for(auto j=i+1UL; j<=m; ++j){
			auto const r_idx = m - j;
			auto const coef = a[r_idx][c_idx];
			a[r_idx][c_idx] = T{0};

			if(coef != 0.0){
				row_operation(a, r_idx, c_idx, T{1}, -coef, c_idx+1UL);
				row_op(r_idx, c_idx, T{1}, -coef);
			}
		}
	}
}

template<typename T, uint64_t Rows, uint64_t Cols>
constexpr
void gaussian_elimination(mat<T, Rows, Cols>& a)
{

	constexpr auto empty_swap = [](auto i, auto j){};
	constexpr auto empty_row = [](auto ri, auto rj, bool alpha, bool beta)
	{};

	gaussian_elimination(a, empty_swap, empty_row);
}

template<typename T, uint64_t Rows, uint64_t Cols>
constexpr auto rank(mat<T, Rows, Cols>& a)
{
	detail::gaussian_elimination(a);

	std::array<uint64_t, Rows> r;
	std::array<uint64_t, Cols> c;

	rgs::fill(r, 0UL);
	rgs::fill(c, 0UL);

	auto n_rows = 0UL, n_cols = 0UL;
	for(auto i=0UL; i<Rows; ++i){
		for(auto j=0UL; j<Cols; ++j){
			if(a[i][j] == T{0}){
				r[i]++;
				c[j]++;

				if(r[i] == Cols)
					++n_rows;
				if(c[j] == Rows)
					++n_cols;
			}
		}
	}

	return std::min(Rows - n_rows, Cols - n_cols);
}

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

} // end of namespace detail

template<typename T, uint64_t Rows, uint64_t Cols>
constexpr auto rank(mat<T, Rows, Cols> const& a)
{
	auto M = a;
	return detail::rank(M);
}

template<typename T, uint64_t Rows, uint64_t Cols>
constexpr auto resolve(
	mat<T, Rows, Cols> const& a,
	mat<T, Rows, 1> const& b)
{
	auto A = a;
	auto B = b;
	detail::resolve(A, B);
	return B;
}

template<typename T, uint64_t N>
constexpr auto inverse(mat<T, N, N> const& m)
	-> std::optional<mat<T, N, N>>
{
	constexpr auto identity = make_identity_matrix<T, N>();
	auto inv = identity;

	auto a = m;

	detail::gaussian_elimination(
		a, 
		[&inv](auto i, auto j){ detail::swap_rows(inv, i, j); },
		[&inv](auto r, auto c, auto alpha, auto beta){
			detail::row_operation(inv, r, c, alpha, beta, 0UL);
		}
	);

	if(a != identity)
		return std::nullopt;

	return inv;
}

};
