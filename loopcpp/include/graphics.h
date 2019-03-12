#pragma once
#include<Shader.h>
#include<Texture.h>
#include<Camera.h>
#include<Mesh.h>
#include<Lighting.h>
#include<unordered_map>
#include<string>

#define MAX_LIGHT_COUNT 20

struct Render_System
{
	std::unordered_map<std::string, Mesh *> meshes;
	std::vector<Mesh_Data *> all_mesh_data;
	std::vector<Shader *> shaders;


	Mesh * light_mesh;
	Shader * light_shader;
	int light_count;
	Light lights[MAX_LIGHT_COUNT];

	void initialize();

	Mesh * get_mesh(const char * mesh_name);
	Mesh_Data * get_mesh_data(const char * mesh_name);
	Shader * get_shader(const char * shader_name);

	void setup_lights(int light_count, Light * lights, glm::mat4 view, glm::mat4 projection);
	void render_mesh(Mesh * mesh, glm::vec4 color, glm::mat4 model, glm::mat4 view, glm::mat4 projection);
};

void setup_mesh_data(Mesh_Data * mesh);
Mesh * prepare_mesh(Mesh_Data * mesh);