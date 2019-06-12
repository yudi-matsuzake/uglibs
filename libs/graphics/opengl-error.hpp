#pragma once

#include <iostream>
#include <fstream>

#include <cstdint>
#include <cassert>

// WARN: include glad.h before glfw3.h
#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace graphics {

void glerror_msg(
	int pre,
	const char* call,
	const char* file,
	long line,
	const char* function,
	const char* error_macro,
	const char* error_description);

void glerror_pre_callback(
	const char* call,
	const char* file,
	long line,
	const char* function,
	const char* error_macro,
	const char* error_description);

void glerror_post_callback(
	const char* call,
	const char* file,
	long line,
	const char* function,
	const char* error_macro,
	const char* error_description);

const char* glerror_get_error_macro(GLenum error);

const char* glerror_get_error_description(GLenum error);

}

#define GL_TEST(x, file, line, pretty_function, callback)	\
	while((error = glGetError()) != GL_NO_ERROR){		\
		callback(					\
			#x,					\
			file,					\
			line,					\
			pretty_function,			\
			graphics::glerror_get_error_macro(error),	\
			graphics::glerror_get_error_description(error)\
		);						\
	}

#define GL_CALL(x, file, line, pretty_function)		\
	do{							\
		GLenum error;					\
		GL_TEST(					\
			x,					\
			file,					\
			line,					\
			pretty_function,			\
			graphics::glerror_pre_callback		\
		);						\
		x;						\
		GL_TEST(					\
			x,					\
			file,					\
			line,					\
			pretty_function,			\
			graphics::glerror_post_callback		\
		);						\
	}while(0)

#define GL(x) GL_CALL(x, __FILE__, __LINE__, __PRETTY_FUNCTION__)
