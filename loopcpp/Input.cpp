#include<input.h>
#include<iostream>
#include<sstream>
#include<Engine.h>

void mouse_move_callback(GLFWwindow * window, double x_pos, double y_pos);
void mouse_scroll_callback(GLFWwindow * window, double x_offset, double y_offset);

namespace 
{
	float MOUSE_SENSITIVITY = 1.0f;
	float SCROLL_SENSITIVITY = 5.0f;
}

Input_Module * create_input_module(GLFWwindow * window)
{
	struct Input_Module * input_module = (struct Input_Module *) malloc(sizeof(struct Input_Module));

	input_module->initialize(window);

	return input_module;
}

void Input_Module::initialize(GLFWwindow * window)
{
	this->window = window;

	//config mouse and cursor
	this->mouse_position_sampled = false;
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
}

bool Input_Module::get_key(INPUT_KEY key)
{
	GLFWwindow * window = this->window;
	switch (key)
	{
		case INPUT_KEY_UP:
			return glfwGetKey(window, GLFW_KEY_UP) || glfwGetKey(window, GLFW_KEY_W);
			break;
		case INPUT_KEY_DOWN:
			return glfwGetKey(window, GLFW_KEY_DOWN) || glfwGetKey(window, GLFW_KEY_S);
			break;
		case INPUT_KEY_RIGHT:
			return glfwGetKey(window, GLFW_KEY_RIGHT) || glfwGetKey(window, GLFW_KEY_D);
			break;
		case INPUT_KEY_LEFT:
			return glfwGetKey(window, GLFW_KEY_LEFT) || glfwGetKey(window, GLFW_KEY_A);
			break;
		case INPUT_KEY_E:
			return glfwGetKey(window, GLFW_KEY_E);
			break;
		case INPUT_KEY_Q:
			return glfwGetKey(window, GLFW_KEY_Q);
			break;
		default:
			return false;
	}

	return false;
}

void mouse_move_callback(GLFWwindow * window, double x_pos, double y_pos)
{
	Camera * camera = main_engine->main_camera;
	Input_Module * input_module = main_engine->input_module;

	float normalized_delta_x = (x_pos - input_module->last_mouse_x) / (main_engine->screen_width);
	float normalized_delta_y = -(y_pos - input_module->last_mouse_y) / (main_engine->screen_height);

	if(input_module->mouse_position_sampled)
	{
		camera->rotate_pitch(normalized_delta_y * MOUSE_SENSITIVITY);
		camera->rotate_yaw(normalized_delta_x * MOUSE_SENSITIVITY);
	}
	else
	{
		input_module->mouse_position_sampled = true;
	}

	input_module->last_mouse_x = x_pos;
	input_module->last_mouse_y = y_pos;
}

void mouse_scroll_callback(GLFWwindow * window, double x_offset, double y_offset)
{
	Camera * camera = main_engine->main_camera;
	
	camera->fov -= y_offset * SCROLL_SENSITIVITY;

	if (camera->fov > 60.0f)
	{
		camera->fov = 60.0f;
	}

	if (camera->fov < 5.0f)
	{
		camera->fov = 5.0f;
	}
}