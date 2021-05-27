#include "ug/graphics/mesh3d-render.hpp"
#include "ug/graphics/program.hpp"
#include "ug/graphics/shader.hpp"

namespace ug::graphics{

static constexpr auto vertex_shaders = std::array{R"__(
#version 330 core

layout (location = 0) in vec3 l_pos;
layout (location = 1) in vec3 l_normal;
layout (location = 2) in vec4 l_color;

out vec3 normal;
out vec3 frag_pos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec4 v_color;

void main()
{
	normal = mat3(transpose(inverse(u_model))) * l_normal;
	frag_pos = vec3(u_model * vec4(l_pos, 1.0));
	gl_Position = u_projection * u_view * u_model * vec4(l_pos, 1.f);
	v_color = l_color;
}
)__"};

static constexpr auto fragment_shaders = std::array{R"__(
#version 330 core

out vec4 frag_color;

in vec3 normal;
in vec3 frag_pos;
in vec4 v_color;

uniform vec3 u_light_pos;
uniform vec3 u_view_pos;
uniform vec3 u_light_color;

void main()
{
	// ambient
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * u_light_color;

	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(u_light_pos - frag_pos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_light_color;

	// specular
	float specular_strength = 0.5;
	vec3 view_dir = normalize(u_view_pos - frag_pos);
	vec3 reflect_dir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
	vec3 specular = specular_strength * spec * u_light_color;  

	vec3 result = (ambient + diffuse + specular) * v_color.rgb;
	frag_color = vec4(result, v_color.a);
} 
)__"};

static auto make_program()
{
	ug::graphics::vertex_shader vs;
	ug::graphics::fragment_shader fs;

	vs.set_source(vertex_shaders);
	fs.set_source(fragment_shaders);

	vs.compile();
	fs.compile();

	ug::graphics::program p;
	p.attach_shader(vs);
	p.attach_shader(fs);
	p.link();

	return p;
}

mesh3d_render::mesh3d_render(app* app_ptr)
	: render(app_ptr), m_program(make_program())
{}

void mesh3d_render::operator()(scene const& s, mesh3d const& mesh)
{
	m_program.use();
	mesh.bind();

	auto const view = s.camera.view();

	auto const [ sw, sh ] = get_app()->get_framebuffer_size();
	auto const screen_width = static_cast<float>(sw);
	auto const screen_height = static_cast<float>(sh);

	auto const projection = glm::perspective(
		glm::radians(45.f),
		screen_width / screen_height,
		.1f, 100.f
	);


	m_program.set_uniform("u_model", mesh.model_matrix() );
	m_program.set_uniform("u_view", view);
	m_program.set_uniform("u_projection", projection);
	m_program.set_uniform("u_light_pos", glm::vec3(3., 3., 3.));
	m_program.set_uniform("u_view_pos", glm::vec3(0., 0., 6.));
	m_program.set_uniform("u_light_color", glm::vec3(1., 1., 1.));

	GL(glDrawElements(
			GL_TRIANGLES,
			static_cast<int>(mesh.attributes().size()),
			GL_UNSIGNED_INT,
			0
	));
}

} // end of namespace ug::graphics
