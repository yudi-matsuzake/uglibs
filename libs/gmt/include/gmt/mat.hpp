#pragma once

#include <ranges>
#include <algorithm>
#include <cinttypes>
#include <array>
#include <vector>

#include "util/misc.hpp"
#include "gmt/point.hpp"
#include "gmt/vector.hpp"

namespace gmt{

namespace rgs = std::ranges;
namespace vws = std::views;

template<class T, uint64_t Row, uint64_t Col>
class mat : public std::array<std::array<T, Col>, Row>{
public:
	using row_t = std::array<T, Col>;

	constexpr mat() : std::array<row_t, Row>{}
	{}

	template<class ... Rows>
		requires (std::is_same_v<Rows, row_t> && ...)
			&& (sizeof...(Rows) == Col)
	constexpr mat(Rows&& ... rows)
	{
		auto i = 0UL;
		(rgs::copy_n(rows.begin(), Col, (*this)[i++].begin()), ...);
	}

	constexpr mat(T const (&a)[Row][Col])
	{
		for(auto i : vws::iota(0UL, Row))
			for(auto j : vws::iota(0UL, Col))
				(*this)[i][j] = a[i][j];
	}

	static constexpr auto n_row = Row;
	static constexpr auto n_col = Col;
};

template<class R, class ... Rows>
	requires ((std::tuple_size_v<R> == std::tuple_size_v<Rows>) && ...)
		&& ((std::is_same_v<std::tuple_element_t<0, R>,
				std::tuple_element_t<0, Rows>>) && ...)
mat(R&&, Rows&&...) -> mat<
	std::tuple_element_t<0, R>,
	sizeof...(Rows)+1,
	std::tuple_size_v<R>
>;

template<class T, class Q, uint64_t N, uint64_t M, uint64_t K>
	requires std::common_with<T, Q>
		&& std::is_same_v<std::common_type_t<T, Q>, Q>
[[nodiscard]] constexpr
auto operator*(mat<T, N, M> const& a, mat<Q, M, K> const& b) -> mat<Q, N, K>
{
	mat<Q, N, K> r;

	for(auto i : vws::iota(0UL, N)){
		for(auto j : vws::iota(0UL, K)){
			r[i][j] = Q{0};

			for(auto k : vws::iota(0UL, M))
				r[i][j] += Q{a[i][k]} * b[k][j];
		}
	}

	return r;
}

template<class T, uint64_t N, class Op>
constexpr auto to_mat(gmt::vector<T, N, Op> const& v)
{
	mat<T, N, 1UL> m;
	for(auto i : vws::iota(0UL, N))
		m[i].front() = v[i];
	return m;
}

template<class T, class Q, uint64_t N, class C>
	requires std::common_with<T, Q>
		&& std::is_same_v<std::common_type_t<T, Q>, Q>
[[nodiscard]] constexpr
auto operator*(mat<T, N+1UL, N+1UL> const& m, point<Q, N, C> const& v) -> C
{

	auto rm = [&]{
		mat<Q, N+1UL, 1UL> vm;
		for(auto i : vws::iota(0UL, N))
			vm[i].front() = v[i];
		vm.back().front() = Q{1};

		return m*vm;
	}();

	C r;
	for(auto i : vws::iota(0UL, N))
		r[i] = rm[i].front();

	return r;
}

template<class T, class Q, uint64_t R, uint64_t C, class Op>
	requires std::common_with<T, Q>
[[nodiscard]] constexpr
auto operator*(mat<T, R, C> const& m, vector<Q, C, Op> const& v)
{
	using result_value_t = std::common_type_t<T, Q>;
	using result_t = vector<
		result_value_t,
		R,
		Op
	>;

	auto rm = m*to_mat(v);

	result_t r;
	for(auto i : vws::iota(0UL, R))
		r[i] = rm[i].front();

	return r;
}

template<class T, uint64_t R, uint64_t C>
[[nodiscard]] constexpr
auto operator*(mat<T, R, C> const& a, T const& s)
{
	using result_t = mat<T, R, C>;

	result_t r;

	for(auto i : vws::iota(0UL, R)){
		for(auto j : vws::iota(0UL, C)){
			r[i][j] = a[i][j] * s;
		}
	}

	return r;
}

template<class T, class Q, uint64_t R, uint64_t C>
	requires std::common_with<T, Q>
[[nodiscard]] constexpr
auto operator*(Q const& s, mat<T, R, C> const& a)
{
	return a*s;
}

template<class T, uint64_t N>
constexpr auto make_identity_matrix()
{
	mat<T, N, N> m;
	for(auto i : vws::iota(0UL, N))
		for(auto j : vws::iota(0UL, N))
			m[i][j] = (i == j)? T{1} : T{0};
	return m;
}

template<class T>
constexpr auto determinant(mat<T, 1, 1> const& m)
{
	return m[0][0];
}

template<class T>
constexpr auto determinant(mat<T, 2, 2> const& m)
{
	return m[0][0]*m[1][1] - m[0][1]*m[1][0];
}

template<class T, uint64_t N>
constexpr auto determinant(mat<T, N, N> const& m)
{
	T det = T{0};

	mat<T, N-1, N-1> sub_matrix;

	auto sign = T{1};

	for(uint64_t i=0; i<N; i++){
		uint64_t subidx = 0;

		for(uint64_t j=1; j<N; j++){
			for(uint64_t k=0; k<N; k++){
				if(k != i){
					uint64_t subi = subidx/sub_matrix.n_row;
					uint64_t subj = subidx%sub_matrix.n_col;
					sub_matrix[subi][subj] = m[j][k];
					subidx++;
				}
			}
		}

		det += sign*m[0][i]*determinant(sub_matrix);
		sign *= T{-1};
	}

	return det;
}

template<class T, uint64_t R, uint64_t C>
constexpr auto transpose(mat<T, R, C> const& m)
{
	mat<T, C, R> t;

	for(auto r : vws::iota(0UL, R))
		for(auto c : vws::iota(0UL, C))
			t[c][r] = m[r][c];

	return t;
}

template<class T, uint64_t N, class C>
constexpr auto make_translation_matrix(vector<T, N, C> const& t)
{
	constexpr auto identity = make_identity_matrix<T, N+1UL>();

	auto tm = identity;
	for(auto i : vws::iota(0UL, N))
		tm[i].back() = t[i];

	return tm;
}

namespace detail{

template<class Matrix, class First, class ... PointType>
[[nodiscard]] inline constexpr
auto make_basis_column_matrix(Matrix& m, uint64_t col, First&& p, PointType&& ... vecs)
	noexcept
{
	for(auto i : vws::iota(0UL, m.n_row))
		m[i][col] = p[i];

	if constexpr(sizeof...(PointType)){
		make_basis_column_matrix(
			m,
			col+1UL,
			std::forward<PointType>(vecs) ...
		);
	}
}

} // end of namespace detail

template<pointlike First, pointlike ... PointType>
[[nodiscard]] constexpr auto make_basis_column_matrix(
	First&& p,
	PointType&& ... vecs) noexcept
{
	using result_t = gmt::mat<
		typename First::element_type,
		First::dim,
		sizeof...(PointType) + 1UL
	>;

	result_t m;
	detail::make_basis_column_matrix(
		m,
		0UL,
		std::forward<First>(p),
		std::forward<PointType>(vecs)...
	);
	return m;
}

template<class T, uint64_t N, class C, uint64_t Size>
[[nodiscard]] constexpr
auto make_basis_column_matrix(std::array<vector<T, N, C>, Size> const& a)
	noexcept
{
	using result_t = gmt::mat<T, N, Size>;

	result_t m;

	for(auto r : vws::iota(0UL, N))
		for(auto c : vws::iota(0UL, Size))
			m[r][c] = a[c][r];

	return m;
}

template<class T, uint64_t R, uint64_t C>
[[nodiscard]] constexpr
auto make_projection_matrix(mat<T, R, C> const& a)
{
	auto const t = transpose(a);
	auto const& inv = inverse(t*a).value();
	return a * inv * t;
}

template<class T, uint64_t R, uint64_t C>
[[nodiscard]] constexpr
auto make_orthonormal_projection_matrix(mat<T, R, C> const& a) noexcept
{
	auto const t = transpose(a);
	return a * t;
}

/**
  * Computes the orthonormal basis for the `N` vectors in `v`
  * using gram-schmidt process
  */
template<class T, uint64_t N, class C, uint64_t Size>
[[nodiscard]]
auto compute_orthonormal_basis(std::array<vector<T, N, C>, Size> const& vs)
{
	using result_t = std::array<vector<T, N, C>, Size>;

	result_t base;

	auto const to_versor = [](auto const& v)
	{
		auto const len = norm(v);
		if(len == T{0}){
			throw std::runtime_error(
				"could not create orthonormal basis with"
				" zero length vector!"
			);
		}

		return (len != T{1})
			? v / len
			: v;
	};

	auto const proj = [](auto const& u, auto const& v)
	{
		return u * (dot(u, v) / dot(u, u));
	};

	base.front() = to_versor(vs.front());

	for(auto i : vws::iota(1UL, Size)){
		auto const proj_sum = [&]{
			auto s = gmt::vector<T, N, C>::all(T{0});
			for(auto j : vws::iota(0UL, i))
				s += proj(base[j], vs[i]);
			return s;
		}();

		base[i] = to_versor(vs[i] - proj_sum);
	}

	return base;
}

template<uint64_t N, std::floating_point T>
auto make_givens_rotation_matrix(uint64_t i, uint64_t j,  T theta)
{
	gmt::mat<T, N, N> m;

	auto const cos = std::cos(theta);
	auto const sin = std::sin(theta);

	for(auto r : vws::iota(0UL, N)){
		for(auto c : vws::iota(0UL, N)){
			m[r][c] = [&]{
				if(r == c){
					if(r == i || r == j)
						return T{cos};
					else
						return T{1};
				}else if((r == i && c == j)
						|| (r == j && c == i)){
					if(c < r)
						return T{sin};
					else
						return T{-sin};
				}

				return T{0};
			}();
		}
	}

	return m;
}

template<class T, uint64_t N, class C>
constexpr auto make_standard_basis()
{
	std::array<vector<T, N, C>, N> b;
	
	for(auto i : vws::iota(0UL, N)){
		for(auto j : vws::iota(0UL, N)){
			if(i == j)
				b[i][j] = T{1};
			else
				b[i][j] = T{0};
		}
	}
	return b;
}

/**
  * receives a basis with `Size` <= `N` vectors with `N` dimensions
  * and enlarge it to `N` vector basis
  */
template<class T, uint64_t N, class C, uint64_t Size>
	requires (Size <= N)
constexpr auto enlarge_basis(std::array<vector<T, N, C>, Size> const& b)
{
	auto vs = util::concatenate_arrays(b, make_standard_basis<T, N, C>());
	mat<T, N, N> m;

	rgs::fill(m, []
	{
		std::array<T, N> a;
		rgs::fill(a, T{0});
		return a;
	}());

	for(auto i=0UL, c_vs=0UL; i<N && c_vs<vs.size(); ++c_vs){
		for(auto j=0; j<N; ++j)
			m[i][j] = vs[c_vs][j];

		if(rank(m) == (i+1UL))
			++i;
	}

	return transpose(m);
}

/* template<std::floating_point T, uint64_t N, class C> */
/* auto make_rotation_matrix( */
/* 	gmt::vector<T, N, C> const& x, */
/* 	gmt::vector<T, N, C> const& y, */
/* 	T theta) */
/* { */
/* 	static_assert( */
/* 		N >= 2, */
/* 		"this rotation matrix is not defined to for dimension" */
/* 		" smaller than 2" */
/* 	); */

/* 	using vec_t = gmt::vector<T, N, C>; */

/* 	auto ortho_basis = compute_orthonormal_basis(std::array{x, y}); */
/* 	auto basis_matrix = make_basis_column_matrix(ortho_basis); */
/* 	auto const xm = resolve(basis_matrix, x); */
/* 	auto const ym = resolve(basis_matrix, y); */

/* 	auto const x_prime = gmt::vector<T, 2, C>{xm[0], xm[1]}; */
/* 	auto const y_prime = gmt::vector<T, 2, C>{ym[0], ym[1]}; */

/* 	auto givens = make_givens_rotation_matrix<2>(0UL, 1UL, theta); */

/* 	/1* auto x_prime = gmt::resolve( *1/ */

/* 	/1* std::array<vec_t, N> n_dimensional_basis; *1/ */

/* 	/1* n_dimensional_basis[0] = ortho_basis[0]; *1/ */
/* 	/1* n_dimensional_basis[1] = ortho_basis[1]; *1/ */

/* 	/1* for(auto&& b : n_dimensional_basis | vws::drop(2)) *1/ */
/* 	/1* 	b = vec_t::all(0.0); *1/ */

/* 	auto base_matrix = make_basis_column_matrix(n_dimensional_basis); */
/* 	auto givens = make_givens_rotation_matrix<N>(0UL, 1UL, theta); */
/* 	auto ins = inverse(base_matrix).value(); */
/* 	return ins * givens * base_matrix; */
/* } */

} // end of namespace gmt
