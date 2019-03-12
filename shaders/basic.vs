#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 vertex_normal;
out vec3 world_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal_matrix;

void main()
{
	gl_Position = projection * view * model * vec4(pos.xyz, 1.0);
	
	//compute world normal
	vertex_normal = normal_matrix * normal;

	world_pos = (model * vec4(pos, 1.0)).xyz;
}