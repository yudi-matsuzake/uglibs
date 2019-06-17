#include <vector>

#include "graphics/misc.hpp"

namespace graphics{

/*
 * check compilation and throw an exception in case
 * failed compilation
 */
void check_shader_compilation(uint32_t id)
{
	int32_t success = 0;
	GL(glGetShaderiv(id, GL_COMPILE_STATUS, &success));

	if(!success){
		int32_t infolog_length = 0;
		GL(glGetShaderiv(
			id,
			GL_INFO_LOG_LENGTH,
			&infolog_length
		));

		std::vector<char> v(static_cast<unsigned>(infolog_length));

		GL(glGetShaderInfoLog(
			id,
			infolog_length,
			NULL,
			v.data()
		));

		throw std::runtime_error(v.data());
	}
}

} // end of namespace graphics
