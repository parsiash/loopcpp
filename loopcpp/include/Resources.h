#pragma once
#include<vector>
#include<Mesh.h>

std::vector<Mesh*> load_model(const char * model_path);
std::vector<Mesh*> load_all_models();