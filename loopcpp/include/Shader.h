#pragma once
#include<glm/glm.hpp>

using namespace glm;

struct Shader
{
	unsigned int program_id;
	void use();

	void set_bool(const char * name, bool vlaue);
	void set_int(const char * name, int value);
	void set_float(const char * name, float value);
	void set_vec3(const char * name, vec3 value);
	void set_mat4(const char * name, mat4 value);
};

Shader * create_shader(const char * vertex_shader_path, const char * fragment_shader_path);