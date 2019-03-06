#pragma once
#include <GLFW/glfw3.h>
#include <input.h>
#include<Camera.h>
#include<Shader.h>
#include<vector>
#include<graphics.h>

struct Engine
{
	GLFWwindow * window;
	int screen_width;
	int screen_height;
	Shader * test_shader;
	unsigned int vbo;
	unsigned int ebo;
	unsigned int test_vao;
	unsigned int texture_0;
	unsigned int texture_1;
	Camera * main_camera;
	float camera_move_speed;
	float last_frame_time;
	struct Input_Module * input_module;

	unsigned int mesh_count;
	Mesh ** meshes;

	void update();
};

struct Engine * create_engine(GLFWwindow * window, int screen_width, int screen_height);


extern Engine * main_engine;