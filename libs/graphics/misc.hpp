#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "graphics/opengl-error.hpp"

namespace graphics{

/**
  * check compilation and throw an exception (runtime_error) in case
  * the shader with id `id` failed to be compiled
  */
void check_shader_compilation(uint32_t id);

} // end of namespace graphics
