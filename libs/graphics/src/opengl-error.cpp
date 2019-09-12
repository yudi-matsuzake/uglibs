#include "ug/graphics/opengl-error.hpp"

namespace graphics {

void glerror_msg(
	int pre,
	const char* call,
	const char* file,
	long line,
	const char* function,
	const char* error_macro,
	const char* error_description)
{
	const char* when = pre?"before":"after";
	/* static std::ofstream error_file("errors.txt"); */

	std::cerr
		<< "OpenGL ERROR ----------------------------------------------"
		<< '\n'
		<< '\t' << error_macro << ": " << error_description << '\n'
		<< '\t' << "ocurred " << when << " the call: " << call << '\n'
		<< '\t' << file << " (" << line << ") in the function "
		<< function << '\n' << std::flush;
}

void glerror_pre_callback(
	const char* call,
	const char* file,
	long line,
	const char* function,
	const char* error_macro,
	const char* error_description)
{

	assert(call != nullptr &&
		file != nullptr &&
		function != nullptr &&
		error_macro != nullptr &&
		error_description != nullptr);
	glerror_msg(
		1,
		call,
		file,
		line,
		function,
		error_macro,
		error_description
	);
}

void glerror_post_callback(
	const char* call,
	const char* file,
	long line,
	const char* function,
	const char* error_macro,
	const char* error_description)
{
	assert(call != nullptr &&
		file != nullptr &&
		function != nullptr &&
		error_macro != nullptr &&
		error_description != nullptr);
	glerror_msg(
		0,
		call,
		file,
		line,
		function,
		error_macro,
		error_description
	);
}

const char* glerror_get_error_macro(GLenum error)
{
	const char* macro = nullptr;
	switch(error){
	case GL_NO_ERROR:
		macro = "GL_NO_ERROR";
		break;
	case GL_INVALID_ENUM:
		macro = "GL_INVALID_ENUM";
		break;
	case GL_INVALID_VALUE:
		macro = "GL_INVALID_VALUE";
		break;
	case GL_INVALID_OPERATION:
		macro = "GL_INVALID_OPERATION";
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		macro = "GL_INVALID_FRAMEBUFFER_OPERATION";
		break;
	case GL_OUT_OF_MEMORY:
		macro = "GL_OUT_OF_MEMORY";
		break;
	case GL_STACK_UNDERFLOW:
		macro = "GL_STACK_UNDERFLOW";
		break;
	case GL_STACK_OVERFLOW:
		macro = "GL_STACK_OVERFLOW";
		break;
	}
	return macro;
}

const char* glerror_get_error_description(GLenum error)
{
	const char* msg = NULL;
	switch(error){
	case GL_NO_ERROR:
		msg =	"No error has been recorded. The value of this symbolic "
			"constant is guaranteed to be 0.";
		break;

	case GL_INVALID_ENUM:
		msg =	"An unacceptable value is specified for an enumerated "
			"argument. The offending command is ignored and has no "
			"other side effect than to set the error flag.";
		break;

	case GL_INVALID_VALUE:
		msg =	"A numeric argument is out of range. The offending "
			"command is ignored and has no other side effect than "
			"to set the error flag.";
		break;

	case GL_INVALID_OPERATION:
		msg =	"The specified operation is not allowed in the current "
			"state. The offending command is ignored and has no "
			"other side effect than to set the error flag.";
		break;

	case GL_INVALID_FRAMEBUFFER_OPERATION:
		msg =	"The framebuffer object is not complete. The offending "
			"command is ignored and has no other side effect than "
			"to set the error flag.";
		break;

	case GL_OUT_OF_MEMORY:
		msg =	"There is not enough memory left to execute the "
			"command. The state of the GL is undefined, except for "
			"the state of the error flags, after this error is "
			"recorded.";
		break;

	case GL_STACK_UNDERFLOW:
		msg =	"An attempt has been made to perform an operation that "
			"would cause an internal stack to underflow.";
		break;

	case GL_STACK_OVERFLOW:
		msg =	"An attempt has been made to perform an operation that "
			"would cause an internal stack to overflow.";
		break;
	}

	return msg;
}

}
