#pragma once

#include "point.hpp"

namespace gmt{

template<class T, uint64_t N>
class sphere{
public:

	sphere() = default;

	sphere(point<T, N> const& a_center, T a_radius)
		: m_center(a_center), m_radius(a_radius)
	{}

	point<T, N>& center()
	{
		return m_center;
	}

	point<T, N> const& center() const
	{
		return m_center;
	}

	T& radius()
	{
		return m_radius;
	}

	T const& radius() const
	{
		return m_radius;
	}

protected:
	point<T, N> m_center;
	T m_radius;
};

}
