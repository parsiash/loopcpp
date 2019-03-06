#include <glad/glad.h>
#include<Engine.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<Input.h>
#include<graphics.h>
#include<Resources.h>
#include<vector>

enum shader_type {vertex, fragment};

using namespace std;

Engine * main_engine;

struct Engine * create_engine(GLFWwindow * asghar, int width, int height)
{
	main_engine = (struct Engine *)malloc(sizeof(struct Engine));
	main_engine->camera_move_speed = 7.0f;

	main_engine->window = asghar;
	main_engine->screen_width = width;
	main_engine->screen_height = height;

	main_engine->test_shader = create_shader("../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");
	cout << "shaders loaded" << endl;

	//initialize sub-systems
	main_engine->input_module = create_input_module(main_engine->window);

	//create camera
	main_engine->main_camera = create_camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	//configure openGL
	glEnable(GL_DEPTH_TEST);
	int max_vertex_attrib;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vertex_attrib);
	//cout << "maximum vertex attributes : " << max_vertex_attrib << endl;


	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	main_engine->texture_0 = generate_texture("../resources/Stone.jpg");
	main_engine->texture_1 = generate_texture("../resources/mahdis.jpg");

	//load models
	vector<Mesh *> temp_meshes;
	temp_meshes = load_model("../resources/cube.dae");
	auto second_meshes = load_model("../resources/building.dae");
	temp_meshes.insert(temp_meshes.end(), second_meshes.begin(), second_meshes.end());
	main_engine->mesh_count = temp_meshes.size();
	main_engine->meshes = (Mesh **)malloc(main_engine->mesh_count * sizeof(Mesh *));
	for(int  i = 0; i < temp_meshes.size(); i++)
	{
		auto mesh = temp_meshes[i];
		main_engine->meshes[i] = mesh;
		setup_mesh(mesh);

		cout << " mesh vertex count " << mesh->vertex_count << endl;
		for (int i = 0; i < mesh->vertex_count; i++)
		{
			auto vertex = mesh->vertices[i];
			cout << "vertex : " << i << " " << vertex.position.x << " " << vertex.position.y << " " << vertex.position.z << endl;
		}

	}

	
	cout << "texture loaded" << endl;

	return main_engine;
}

void process_input(struct Engine * engine, float dt)
{
	//close by escape
	if (glfwGetKey(engine->window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(engine->window, true);
		return;
	}


	//handle camera movement
	if (engine->input_module->get_key(INPUT_KEY_UP))
	{
		engine->main_camera->move(engine->main_camera->forward * engine->camera_move_speed * dt);
	}
	if (engine->input_module->get_key(INPUT_KEY_DOWN))
	{
		engine->main_camera->move(-engine->main_camera->forward * engine->camera_move_speed * dt);
	}

	if (engine->input_module->get_key(INPUT_KEY_RIGHT))
	{
		engine->main_camera->move(engine->main_camera->get_right() * engine->camera_move_speed * dt);
	}

	if (engine->input_module->get_key(INPUT_KEY_LEFT))
	{
		engine->main_camera->move(-engine->main_camera->get_right() * engine->camera_move_speed * dt);
	}
}

void render(struct Engine * engine)
{
	glm::vec3 cube_positions[] = {
	  glm::vec3(0.0f,  0.0f,  0.0f),
	  glm::vec3(2.0f,  5.0f, -15.0f),
	  glm::vec3(-1.5f, -2.2f, -2.5f),
	  glm::vec3(-3.8f, -2.0f, -12.3f),
	  glm::vec3(2.4f, -0.4f, -3.5f),
	  glm::vec3(-1.7f,  3.0f, -7.5f),
	  glm::vec3(1.3f, -2.0f, -2.5f),
	  glm::vec3(1.5f,  2.0f, -2.5f),
	  glm::vec3(1.5f,  0.2f, -1.5f),
	  glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glClearColor(1, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, engine->texture_0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, engine->texture_1);

	engine->test_shader->use();
	glUniform1i(glGetUniformLocation(engine->test_shader->program_id, "texture_0"), 0);
	glUniform1i(glGetUniformLocation(engine->test_shader->program_id, "texture_1"), 1);


	//update view transform
	glm::mat4 view_transform = engine->main_camera->get_view_transform();

	glm::mat4 projection_transform = glm::perspective(glm::radians(engine->main_camera->fov), (engine->screen_width * 1.0f) / engine->screen_height, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(engine->test_shader->program_id, "view"), 1, GL_FALSE, glm::value_ptr(view_transform));
	glUniformMatrix4fv(glGetUniformLocation(engine->test_shader->program_id, "projection"), 1, GL_FALSE, glm::value_ptr(projection_transform));

	for (int i = 0; i < main_engine->mesh_count; i++)
	{
		Mesh * mesh = main_engine->meshes[i];
		glBindVertexArray(mesh->vao);

		glm::mat4 model_transform = glm::mat4(1.0f);
		model_transform = glm::translate(model_transform, cube_positions[i]);
		glUniformMatrix4fv(glGetUniformLocation(engine->test_shader->program_id, "model"), 1, GL_FALSE, glm::value_ptr(model_transform));

		glDrawArrays(GL_TRIANGLES, 0, mesh->vertex_count);
	}

}


void Engine::update()
{
	//update time
	float current_time = glfwGetTime();
	float dt = current_time - this->last_frame_time;
	this->last_frame_time = current_time;

	process_input(this, dt);
	render(this);
}
