#pragma once

struct Shader
{
	unsigned int program_id;
	void use();
};

Shader * create_shader(const char * vertex_shader_path, const char * fragment_shader_path);