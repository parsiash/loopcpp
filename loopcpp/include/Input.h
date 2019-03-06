#pragma once
#include<GLFW/glfw3.h>

enum INPUT_KEY
{
	INPUT_KEY_UP,
	INPUT_KEY_DOWN,
	INPUT_KEY_RIGHT,
	INPUT_KEY_LEFT
};

struct Input_Module
{
	GLFWwindow * window;

	bool mouse_position_sampled;
	float last_mouse_x;
	float last_mouse_y;

	void initialize(GLFWwindow * window);
	bool get_key(INPUT_KEY key);
};

Input_Module * create_input_module(GLFWwindow * window);