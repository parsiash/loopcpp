#pragma once
#include<glm/glm.hpp>

struct Camera
{
	//basic vectors
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;

	float fov;

	//rotation values
	float pitch;
	float yaw;

	glm::mat4 get_view_transform();
	void move(glm::vec3 move_vector);
	glm::vec3 get_target();
	glm::vec3 get_right();
	void rotate_pitch(float amount);
	void rotate_yaw(float amount);
	void update_forward();
};

struct Camera * create_camera(glm::vec3 position, glm::vec3 target);
