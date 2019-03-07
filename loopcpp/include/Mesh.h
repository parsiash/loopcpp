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
	int mesh_id;
	char name[20];

	//vertex data
	unsigned int vertex_count;
	Vertex * vertices;
	
	//render data
	unsigned int vao;
	unsigned int vbo;
};

struct Rendered_Mesh
{
	int mesh_id;
	unsigned int vao;
};