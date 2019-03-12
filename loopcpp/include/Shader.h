#pragma once
#include<glm/glm.hpp>

using namespace glm;

struct Shader
{
	char * name;
	unsigned int program_id;
	void use();

	void set_bool(const char * name, bool vlaue);
	void set_int(const char * name, int value);
	void set_float(const char * name, float value);
	void set_vec3(const char * name, vec3 value);
	void set_vec4(const char * name, vec4 value);
	void set_mat4(const char * name, mat4 value);
	void set_mat3(const char * name, mat3 value);
	void set_material(struct Material material);
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

Shader * create_shader(const char * shader_name, const char * vertex_shader_path, const char * fragment_shader_path);