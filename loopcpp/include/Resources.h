#pragma once
#include<vector>
#include<Mesh.h>

std::vector<Mesh_Data*> load_model(const char * model_path);
std::vector<Mesh_Data*> load_all_models();
int get_mesh_id(const char * mesh_name);
Mesh_Data * get_mesh(int mesh_id);