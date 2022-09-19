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

	for(uint64_t c=0UL, r=0UL, first_candidate=0UL; c<a.n_col; c++){

		/*
		 * search for the maximal value in this column
		 */
		auto pivot = [&]{
			for(auto j=first_candidate; j<a.n_row; ++j)
				if(a[j][c] != T{0})
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
		if(r != pivot){
			swap_rows(a, r, pivot);
			swap_op(r, pivot);
			pivot = r;
		}

		first_candidate = pivot + 1UL;
		++r;

		/*
		 * make pivot element equal to 1
		 */
		if(a[pivot][c] != T{1}){
			auto const coef = 1./a[pivot][c];
			a[pivot][c] = T{1};
			row_operation(a, pivot, pivot, T{0}, coef, c+1);
			row_op(pivot, pivot, T{0}, coef);
		}

		/*
		 * Make all cells below the pivot element
		 * equal to zero
		 */
		for(auto j : vws::iota(pivot+1, a.n_row)){
			auto const coef = a[j][c];
			if(coef != T{0}){
				a[j][c] = T{0};
				row_operation(a, j, pivot, T{1}, -coef, c+1);
				row_op(j, pivot, T{1}, -coef);
			}
		}

	}

	/*
	 * make all elements above diagonal equal to zero
	 */
	for(auto r : vws::iota(0UL, a.n_row) | vws::reverse){
		auto const pivot_indices = vws::iota(0UL, a.n_col);
		auto const pivot = rgs::find_if(
			pivot_indices,
			[&a, r](auto i){ return a[r][i] != T{0}; }
		);

		if(pivot == pivot_indices.end())
			continue;

		for(auto i : vws::iota(0UL, r) | vws::reverse){
			auto const coef = a[i][*pivot];
			a[i][*pivot] = T{0};

			if(coef != T{0}){
				row_operation(a, i, r, T{1}, -coef, *pivot+1UL);
				row_op(i, r, T{1}, -coef);
			}
		}
	}
}

template<typename T, uint64_t Rows, uint64_t Cols>
constexpr
void gaussian_elimination(mat<T, Rows, Cols>& a)
{

	constexpr auto empty_swap = [](auto&&, auto&&){};
	constexpr auto empty_row = [](auto&&, auto&&, bool, bool)
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
	for(auto i : vws::iota(0UL, Rows)){
		for(auto j : vws::iota(0UL, Cols)){
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

template<typename T, uint64_t Rows, uint64_t Cols>
constexpr auto gaussian_elimination(mat<T, Rows, Cols> const& a)
{
	auto A = a;
	detail::gaussian_elimination(A);
	return A;
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
