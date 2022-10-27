#pragma once

#include <concepts>

#include "glm/vec2.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/transform.hpp"

#include "ug/graphics/opengl-error.hpp"


namespace ug::graphics{

/*
 * structs and aliases
 * ===================
 */
using color4 = glm::vec4;
using color3 = glm::vec3;
using scalar = double;

enum class color_type : uint32_t {
	R = GL_RED,
	RG = GL_RG,
	RGB = GL_RGB,
	RGBA = GL_RGBA,
	BGR = GL_BGR,
	BGRA = GL_BGRA
};

template<class T>
struct type_code;

template<>
struct type_code<uint8_t>
{
	static constexpr uint32_t value = GL_UNSIGNED_BYTE;
};

template<>
struct type_code<uint16_t>
{
	static constexpr uint32_t value = GL_UNSIGNED_SHORT;
};

template<>
struct type_code<uint32_t>
{
	static constexpr uint32_t value = GL_UNSIGNED_INT;
};

template<>
struct type_code<float>
{
	static constexpr uint32_t value = GL_FLOAT;
};

template<class T>
static constexpr auto type_code_v = type_code<T>::value;

struct rect2d{
	glm::vec2 position;
	float width, height;

	friend void operator*=(rect2d& r, float s);
	friend void operator+=(rect2d& r, glm::vec2 const& v);
	friend void operator-=(rect2d& r, glm::vec2 const& v);

	glm::vec2 center() const;
	glm::vec2 size() const;

	float left() const;
	float right() const;
	float bottom() const;
	float top() const;
};

template<class IntegerType>
constexpr auto to_closest_integer(scalar t)
{
	auto f = t < scalar{0} ? scalar{-0.5} : scalar{+0.5};
	return static_cast<IntegerType>(t + f);
}

} // end of namespace ug::graphics

