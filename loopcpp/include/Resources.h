#pragma once
#include<vector>
#include<Mesh.h>
#include<graphics.h>

std::vector<Mesh_Data*> load_model(const char * model_path);
std::vector<Mesh_Data*> load_all_models();
Shader * load_shader(const char * shader_name);
std::vector<Shader *> load_all_shaders();