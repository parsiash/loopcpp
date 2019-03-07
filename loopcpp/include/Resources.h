#pragma once
#include<vector>
#include<Mesh.h>

std::vector<Mesh*> load_model(const char * model_path);
std::vector<Mesh*> load_all_models();
int get_mesh_id(const char * mesh_name);
Mesh * get_mesh(int mesh_id);