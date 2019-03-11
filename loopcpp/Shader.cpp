#include<Shader.h>
#include<iostream>
#include<stdlib.h>
#include<file_utility.h>
#include<glad/glad.h>

Shader * create_shader(const char * shader_name, const char * vertex_shader_path, const char * fragment_shader_path)
{
	Shader * shader = (struct Shader *)malloc(sizeof(struct Shader));

	int shader_name_len = strlen(shader_name);
	shader->name = (char *)malloc((shader_name_len) * sizeof(char));
	strcpy(shader->name, shader_name);

	const char * vertex_source = get_file_content(vertex_shader_path);
	const char * fragment_source = get_file_content(fragment_shader_path);

	unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertex_shader_id, 1, &vertex_source, NULL);
	glShaderSource(fragment_shader_id, 1, &fragment_source, NULL);

	glCompileShader(vertex_shader_id);
	glCompileShader(fragment_shader_id);

	int vertex_compile_success, fragment_compile_success;
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &vertex_compile_success);
	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &fragment_compile_success);

	char infoLog[512];

	if (!vertex_compile_success)
	{
		std::cout << "failed to compile vertex shader : " << vertex_shader_path << std::endl;
		return nullptr;
	}


	if (!fragment_compile_success)
	{
		std::cout << "failed to compile fragment shader : " << fragment_shader_path << std::endl;
		return nullptr;
	}

	shader->program_id = glCreateProgram();

	glAttachShader(shader->program_id, vertex_shader_id);
	glAttachShader(shader->program_id, fragment_shader_id);

	glLinkProgram(shader->program_id);
	int success;
	glGetProgramiv(shader->program_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader->program_id, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << std::endl;
	}

	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	return shader;
}

void Shader::use()
{
	glUseProgram(this->program_id);
}

void Shader::set_bool(const char * name, bool value)
{
	int uniform_location = glGetUniformLocation(this->program_id, name);
	if (uniform_location < 0)
	{
		std::cout << "no uniform with name " << name << " found" << std::endl;
		return;
	}

	glUniform1i(uniform_location, (int)value);
}

void Shader::set_int(const char * name, int value)
{
	int uniform_location = glGetUniformLocation(this->program_id, name);
	if (uniform_location < 0)
	{
		std::cout << "no uniform with name " << name << " found" << std::endl;
		return;
	}

	glUniform1i(uniform_location, value);
}

void Shader::set_float(const char * name, float value)
{
	int uniform_location = glGetUniformLocation(this->program_id, name);
	if (uniform_location < 0)
	{
		std::cout << "no uniform with name " << name << " found" << std::endl;
		return;
	}

	glUniform1f(uniform_location, value);
}

void Shader::set_vec3(const char * name, vec3 value)
{
	int uniform_location = glGetUniformLocation(this->program_id, name);
	if (uniform_location < 0)
	{
		std::cout << "no uniform with name " << name << " found" << std::endl;
		return;
	}

	glUniform3fv(uniform_location, 1, &value[0]);
}

void Shader::set_mat4(const char * name, mat4 value)
{
	int uniform_location = glGetUniformLocation(this->program_id, name);
	if (uniform_location < 0)
	{
		std::cout << "no uniform with name " << name << " found" << std::endl;
		return;
	}

	glUniformMatrix4fv(uniform_location, 1, GL_FALSE, &value[0][0]);
}