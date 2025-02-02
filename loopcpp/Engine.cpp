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
#include<Windows.h>
#include<new>


enum shader_type {vertex, fragment};

using namespace std;

Engine * main_engine;

//test paramteres for rendering
float cube_angle_x = 0.0f;
float cube_angle_y = 0.0f;
Material cube_material;
Light light;

struct Engine * create_engine(GLFWwindow * asghar, int width, int height)
{
	main_engine = (struct Engine *)malloc(sizeof(struct Engine));
	main_engine->camera_move_speed = 7.0f;

	main_engine->window = asghar;
	main_engine->screen_width = width;
	main_engine->screen_height = height;

	//initialize sub-systems
	main_engine->input_module = create_input_module(main_engine->window);

	main_engine->render_system = new (malloc(sizeof(Render_System))) Render_System;
	main_engine->render_system->initialize();

	//create camera
	main_engine->main_camera = create_camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	//configure openGL
	glEnable(GL_DEPTH_TEST);
	int max_vertex_attrib;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_vertex_attrib);
	//cout << "maximum vertex attributes : " << max_vertex_attrib << endl;


	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	main_engine->texture_0 = generate_texture("../resources/Stone.jpg");
	main_engine->texture_1 = generate_texture("../resources/mahdis.jpg");


	//initialize cube material
	cube_material.ambient = vec3(0.19225f, 0.19225f, 0.19225f);
	cube_material.diffuse = vec3(0.50754f, 0.50754f, 0.50754f);
	cube_material.specular = vec3(0.2f, 0.2f, 0.2f);
	cube_material.shininess = 128.0f * 0.4f;

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


	//handle test building rotation
	if (engine->input_module->get_key(INPUT_KEY_E))
	{
		cube_angle_x += 2.0f * dt;
	}
	if (engine->input_module->get_key(INPUT_KEY_Q))
	{
		cube_angle_y += 2.0f * dt;
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

	glClearColor(0.05f, 0.1f, 0.12f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	//setup shader and textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, engine->texture_0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, engine->texture_1);

	//glUniform1i(glGetUniformLocation(engine->test_shader->program_id, "texture_0"), 0);
	//glUniform1i(glGetUniformLocation(engine->test_shader->program_id, "texture_1"), 1);


	//setup view and projection transforms
	glm::mat4 view_transform = engine->main_camera->get_view_transform();
	glm::mat4 projection_transform = glm::perspective(glm::radians(engine->main_camera->fov), (engine->screen_width * 1.0f) / engine->screen_height, 0.1f, 100.0f);


	light.position = cube_positions[5];
	light.ambient = vec3(0.1f);
	light.diffuse = vec3(1.0f, 1.0f, 1.0f);
	light.specular = vec3(1.0f);

	//setup lights
	Light lights[1];
	lights[0] = light;
	main_engine->render_system->setup_lights(1, lights, view_transform, projection_transform);

	auto cube_mesh = main_engine->render_system->get_mesh("Head");
	glm::mat4 cube_transform = glm::mat4(1.0f);
	cube_transform = glm::translate(cube_transform, cube_positions[3]);
	cube_transform = glm::rotate(cube_transform, cube_angle_x, vec3(1.0f, 0.0f, 0.0f));
	cube_transform = glm::rotate(cube_transform, cube_angle_y, vec3(0.0f, 1.0f, 0.0f));
	main_engine->render_system->render_mesh(cube_mesh, cube_material, engine->main_camera->position, cube_transform, view_transform, projection_transform);
	//render test light sphere


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
