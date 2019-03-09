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
	std::unordered_map<int, Mesh *> meshes;

	std::unordered_map<std::string, int> mesh_id_map;
	std::vector<Mesh_Data *> all_mesh_data;


	void initialize();
	void render_mesh(Mesh * mesh, glm::mat4 model, glm::mat4 view, glm::mat4 projection, Shader * shader);
};

void setup_mesh_data(Mesh_Data * mesh);
Mesh * prepare_mesh(Mesh_Data * mesh);