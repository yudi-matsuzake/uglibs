#pragma once

#include <ranges>
#include <algorithm>
#include <cinttypes>
#include <array>

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
		for(auto i=0UL; i<Row; ++i)
			for(auto j=0UL; j<Col; ++j)
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

template<class T, uint64_t N>
constexpr auto make_identity_matrix()
{
	mat<T, N, N> m;
	for(auto i=0UL; i<N; ++i)
		for(auto j=0UL; j<N; ++j)
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


} // end of namespace gmt
