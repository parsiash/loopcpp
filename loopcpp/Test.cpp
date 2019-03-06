#include<Resources.h>
#include<iostream>
#include<xml.h>
#include<string_utility.h>
#include<Mesh.h>
#include<file_utility.h>

using namespace std;

int main()
{
	auto meshes = load_model("../resources/cube.dae");

	cout << "mesh count : " << meshes.size() << endl;

	for (auto mesh : meshes)
	{
		for (int i = 0; i < mesh->vertex_count; i++)
		{
			auto vertex_pos = mesh->vertices[i].position;
			cout << "vertex " << i << " : " << vertex_pos.x << " " << vertex_pos.y << " " << vertex_pos.z  << endl;
			cout << "vertex " << i << " : " << vertex_pos.x << " " << vertex_pos.y << " " << vertex_pos.z << endl;
		}
	}
	getchar();
	return 0;
}