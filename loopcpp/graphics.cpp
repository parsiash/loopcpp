#include<graphics.h>
#include<glad\glad.h>
#include<Resources.h>
#include<iostream>
#include<glm/gtc/matrix_transform.hpp>

using namespace std;


void Render_System::initialize()
{
	//load shaders
	this->shaders = load_all_shaders();
	cout << "shaders count = " << this->shaders.size() << endl;

	//load all mesh resources
	vector<Mesh_Data *> loaded_meshes = load_all_models();

	//setup meshes
	for (int i = 0; i < loaded_meshes.size(); i++)
	{
		//setup mesh data in vertex buffer
		Mesh_Data * mesh_data = loaded_meshes[i];
		setup_mesh_data(mesh_data);


		mesh_data->mesh_id = i;
		this->all_mesh_data.push_back(mesh_data);

		//create renderable mesh from mesh data
		auto renderable_mesh = prepare_mesh(mesh_data);
		this->meshes[mesh_data->name] = renderable_mesh;
	}

	//initialize light mesh
	unsigned int light_vao;
	glGenVertexArrays(1, &light_vao);
	glBindVertexArray(light_vao);

	auto sphere_mesh = get_mesh_data("Sphere");
	glBindBuffer(GL_ARRAY_BUFFER, sphere_mesh->vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	this->light_mesh = (Mesh *)malloc(sizeof(Mesh));
	this->light_mesh->vao = light_vao;
	this->light_mesh->mesh_id = sphere_mesh->mesh_id;
	this->light_mesh->vertex_count = sphere_mesh->vertex_count;

	this->light_count = 0;
}

void Render_System::render_mesh(Mesh * mesh, Material material, vec3 camera_position, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	Shader * shader = this->shaders[0];

	//setup shader, color and transforms
	shader->use();
	shader->set_mat4("model", model);
	shader->set_mat4("view", view);
	shader->set_mat4("projection", projection);
	shader->set_material(material);

	//set light-related uniforms
	mat3 normal_matrix = glm::transpose(glm::inverse(model));
	shader->set_mat3("normal_matrix", normal_matrix);
	if (this->light_count > 0)
	{
		shader->set_vec3("light_color", this->lights[0].color);
		shader->set_vec3("light_pos", this->lights[0].position);
	}
	shader->set_vec3("eye_pos", camera_position);
	
	//bind vertex array and draw the mesh
	glBindVertexArray(mesh->vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh->vertex_count);

	glBindVertexArray(0);
}

Mesh_Data * Render_System::get_mesh_data(const char * mesh_name)
{
	for (auto mesh_data : this->all_mesh_data)
	{
		if (!strcmp(mesh_data->name, mesh_name))
		{
			return mesh_data;
		}
	}

	return nullptr;
}

Mesh * Render_System::get_mesh(const char * mesh_name)
{
	if (this->meshes.find(mesh_name) == this->meshes.end())
	{
		return nullptr;
	}

	Mesh * mesh = this->meshes[mesh_name];
	return mesh;
}

Shader * Render_System::get_shader(const char * shader_name)
{
	for (auto shader : this->shaders)
	{
		if (!strcmp(shader->name, shader_name))
		{
			return shader;
		}
	}

	return nullptr;
}



void setup_mesh_data(Mesh_Data * mesh)
{
	unsigned int vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, mesh->vertex_count * sizeof(Vertex), mesh->vertices, GL_STATIC_DRAW);
	
	//unbind gl objects
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	mesh->vbo = vbo;
}


Mesh * prepare_mesh(Mesh_Data * mesh)
{
	if (!mesh || mesh->vbo == 0)
	{
		return nullptr;
	}

	unsigned int vao;
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);

	//configure position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	//configure normal data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//configure position data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Mesh * renderable_mesh = (Mesh *)malloc(sizeof(Mesh));
	renderable_mesh->mesh_id = mesh->mesh_id;
	renderable_mesh->vertex_count = mesh->vertex_count;
	renderable_mesh->vao = vao;

	return renderable_mesh;
}

void Render_System::setup_lights(int light_count, Light * lights, mat4 view, mat4 projection)
{
	//setting lightss
	light_count = min(light_count, MAX_LIGHT_COUNT);
	this->light_count = light_count;
	for (int i = 0; i < light_count; i++)
	{
		this->lights[i] = lights[i];
	}

	//render lights
	glBindVertexArray(this->light_mesh->vao);
	this->light_shader = get_shader("light");
	this->light_shader->use();
	this->light_shader->set_mat4("view", view);
	this->light_shader->set_mat4("projection", projection);
	for (int i = 0; i < this->light_count; i++)
	{
		mat4 model = mat4(1.0f);
		glm::translate(model, this->lights[i].position);
		this->light_shader->set_vec3("light_color", this->lights[i].color);
		this->light_shader->set_mat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, this->light_mesh->vertex_count);
	}
	glBindVertexArray(0);
}