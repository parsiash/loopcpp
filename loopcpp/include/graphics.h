#pragma once
#include<Shader.h>
#include<Texture.h>
#include<Camera.h>
#include<Mesh.h>
#include<Lighting.h>
#include<unordered_map>
#include<string>

struct Render_System
{
	Mesh * light_mesh;
	std::unordered_map<std::string, Mesh *> meshes;
	std::vector<Mesh_Data *> all_mesh_data;

	std::vector<Shader *> shaders;

	void initialize();
	void render_mesh(Mesh * mesh, glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	Mesh * get_mesh(const char * mesh_name);
	Mesh_Data * get_mesh_data(const char * mesh_name);
};

void setup_mesh_data(Mesh_Data * mesh);
Mesh * prepare_mesh(Mesh_Data * mesh);