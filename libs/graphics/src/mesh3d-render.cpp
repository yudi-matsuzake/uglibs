#include "ug/graphics/mesh3d-render.hpp"
#include "ug/graphics/program.hpp"
#include "ug/graphics/shader.hpp"

namespace ug::graphics{

static constexpr auto phong_simple_vs = std::array{R"__(
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

static constexpr auto phong_simple_fs = std::array{R"__(
#version 330 core

out vec4 frag_color;

in vec3 normal;
in vec3 frag_pos;
in vec4 v_color;

uniform vec3 u_light_pos;
uniform vec3 u_view_pos;
uniform vec3 u_light_color;
uniform vec4 u_color;

uniform float u_ambient_light_strength;
uniform float u_specular_strength;
uniform bool u_specular_lighting;
uniform bool u_diffuse_lighting;
uniform bool u_use_attr_color;

void main()
{
	vec3 light = vec3(0.f, 0.f, 0.f);
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(u_light_pos - frag_pos);

	// ambient
	light += u_ambient_light_strength * u_light_color;

	// diffuse
	if(u_diffuse_lighting){
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * u_light_color;
		light += diffuse;
	}

	// specular
	if(u_specular_lighting){
		vec3 view_dir = normalize(u_view_pos - frag_pos);
		vec3 reflect_dir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
		vec3 specular = u_specular_strength * spec * u_light_color;  
		light += specular;
	}

	vec4 color = u_use_attr_color ? v_color : u_color;

	vec3 result = clamp(light, 0.f, 1.f) * color.rgb;
	frag_color = vec4(result, color.a);
} 
)__"};

template<uint64_t N>
static auto make_program(
	static_shader_source<N> const& vertex_shaders,
	static_shader_source<N> const& fragment_shaders)
{
	vertex_shader vs;
	fragment_shader fs;

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
	: render(app_ptr),
	  m_program(make_program(phong_simple_vs, phong_simple_fs))
{}

void mesh3d_render::common_program_setup(scene const& s, mesh3d const& mesh)
{
	m_program.use();
	mesh.bind();

	auto const view = s.camera.view();
	auto const projection = get_app()->compute_projection_matrix();
	auto const& light = s.lights.at(0);

	m_program.set_uniform("u_model", mesh.model_matrix() );
	m_program.set_uniform("u_view", view);
	m_program.set_uniform("u_projection", projection);
	m_program.set_uniform("u_view_pos", s.camera.position);

	m_program.set_uniform("u_light_pos", light.position);
	m_program.set_uniform("u_light_color", light.color);

	m_program.set_uniform("u_ambient_light_strength", ambient_light_strength);
	m_program.set_uniform("u_diffuse_lighting", diffuse_lighting);
	m_program.set_uniform("u_specular_lighting", specular_lighting);
	m_program.set_uniform("u_specular_strength", specular_strength);
}

void mesh3d_render::operator()(
	scene const& s,
	mesh3d const& mesh)
{
	if(triangles.draw){
		common_program_setup(s, mesh);

		m_program.set_uniform("u_color", triangles.color);
		m_program.set_uniform(
			"u_use_attr_color",
			triangles.use_attr_color
		);
		mesh.draw_triangles();
	}

	if(lines.draw){
		common_program_setup(s, mesh);
		float saved_line_width;
		GL(glGetFloatv(GL_LINE_WIDTH, &saved_line_width));

		GL(glLineWidth(lines.line_width));
		m_program.set_uniform("u_color", lines.color);
		m_program.set_uniform(
			"u_use_attr_color",
			lines.use_attr_color
		);
		mesh.draw_lines();

		GL(glLineWidth(saved_line_width));
	}

}

} // end of namespace ug::graphics
