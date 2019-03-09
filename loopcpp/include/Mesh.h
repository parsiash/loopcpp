#pragma once
#include<glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct Mesh_Data
{
	int mesh_id;
	char name[20];

	//vertex data
	unsigned int vertex_count;
	Vertex * vertices;
	
	//render data
	unsigned int vbo;
};

struct Mesh
{
	int mesh_id;
	unsigned int vertex_count;
	unsigned int vao;
};