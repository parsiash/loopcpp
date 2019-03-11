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
	Render_System * render_system;
	unsigned int texture_0;
	unsigned int texture_1;
	Camera * main_camera;
	float camera_move_speed;
	float last_frame_time;
	struct Input_Module * input_module;

	void update();
};

struct Engine * create_engine(GLFWwindow * window, int screen_width, int screen_height);


extern Engine * main_engine;