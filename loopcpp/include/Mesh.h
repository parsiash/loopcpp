#pragma once
#include<glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct Mesh
{
	//vertex data
	unsigned int vertex_count;
	Vertex * vertices;
	
	//render data
	unsigned int vao;
	unsigned int vbo;
};