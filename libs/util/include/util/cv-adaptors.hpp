/** @file cv-adaptors.hpp
  * Implementation of adaptors to help common 
  * opencv objects manipulation
  */

#pragma once
#include <type_traits>

#include <opencv2/core/core.hpp>

namespace cv{

template <bool C, class A, class B>
struct type_if_else{};

template <class A, class B>
struct type_if_else<true, A, B>{
	typedef A type;
};

template <class A, class B>
struct type_if_else<false, A, B>{
	typedef B type;
};

/*
 * adaptors for opencv
 * -------------------
 */

template<class pixel_type, class M>
class cells_iterator_adaptor{
public:
	cells_iterator_adaptor() = delete;
	virtual ~cells_iterator_adaptor() = default;

	explicit cells_iterator_adaptor(M& a_reference) noexcept
		: m_reference(a_reference)
	{}

	auto begin()
	{
		return m_reference.template begin<pixel_type>();
	}

	auto begin() const
	{
		return m_reference.template begin<pixel_type>();
	}

	auto end()
	{
		return m_reference.template end<pixel_type>();
	}

	auto end() const
	{
		return m_reference.template end<pixel_type>();
	}

protected:
	M& m_reference;
};

/**
  * build an adaptor to transverse the cells of an opencv image
  */
template<class T>
auto cells(cv::Mat& img)
{
	return cells_iterator_adaptor<T, cv::Mat>(img);
}

/**
  * build an adaptor to transverse the cells of an opencv image
  */
template<class T>
auto cells(cv::Mat const& img)
{
	return cells_iterator_adaptor<T, cv::Mat const>(img);
}

template<class T, class I, class M>
class pixel_point_iterator : public I {
protected:
	// type of the reference that will be returned in operator*
	using return_reference_type = typename type_if_else<
		std::is_same<I, cv::MatIterator_<T>>::value,
		T,
		T const
	>::type;

public:
	pixel_point_iterator() = delete;
	virtual ~pixel_point_iterator() = default;

	pixel_point_iterator(
		I it,
		cv::Point2i& a_coordinate,
		M& a_img)
		: I(it),
		  m_coordinate(a_coordinate),
		  m_rows(a_img.rows),
		  m_cols(a_img.cols)
	{}

	pixel_point_iterator& operator++()
	{
		++m_coordinate.x;
		if(m_coordinate.x == m_cols){
			m_coordinate.x = 0;
			++m_coordinate.y;
		}

		I::operator++();

		return *this;
	}

	pixel_point_iterator operator++(int)
	{
		++m_coordinate.x;
		if(m_coordinate.x == m_cols){
			m_coordinate.x = 0;
			++m_coordinate.y;
		}

		I::operator++(0);

		return *this;
	}

	std::tuple<return_reference_type&, cv::Point2i&> operator*()
	{
		return { I::operator*(), m_coordinate };
	}

protected:
	cv::Point2i& m_coordinate;
	int32_t m_rows;
	int32_t m_cols;

};

template<class T, class I, class M>
class pixel_point_iterator_adaptor{
public:
	pixel_point_iterator_adaptor() = delete;
	virtual ~pixel_point_iterator_adaptor() = default;

	pixel_point_iterator_adaptor(M& a_img)
		: m_reference(a_img),
		  m_point{0, 0}
	{}

	auto begin()
	{
		return pixel_point_iterator<T, I, M>(
			m_reference.template begin<T>(),
			m_point,
			m_reference
		);
	}

	auto begin() const
	{
		return pixel_point_iterator<T, I, M>(
			m_reference.template begin<T>(),
			m_point,
			m_reference
		);
	}

	auto end()
	{
		return pixel_point_iterator<T, I, M>(
			m_reference.template end<T>(),
			m_point,
			m_reference
		);
	}

	auto end() const
	{
		return pixel_point_iterator<T, I, M>(
			m_reference.template end<T>(),
			m_point,
			m_reference
		);
	}

protected:
	M& m_reference;
	cv::Point2i m_point;
};

/**
  * build an adaptor to transverse the cells and the coordinate 
  * of an opencv image
  */
template<class T>
auto cells_at(cv::Mat& img)
{
	return pixel_point_iterator_adaptor<
		T,
		cv::MatIterator_<T>,
		cv::Mat
	>(img);
}

/**
  * build an adaptor to transverse the cells and the coordinate 
  * of an opencv image
  */
template<class T>
auto cells_at(cv::Mat const& img)
{
	return pixel_point_iterator_adaptor<
		T,
		cv::MatConstIterator_<T>,
		cv::Mat const
	>(img);
}

}
