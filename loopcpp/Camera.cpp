#include<Camera.h>
#include<glm/gtc/matrix_transform.hpp>

Camera * create_camera(glm::vec3 position, glm::vec3 target)
{
	Camera * camera = (struct Camera *) malloc(sizeof(struct Camera));
	
	camera->position = position;
	camera->forward = glm::normalize(target - position);
	camera->up = glm::vec3(0.0f, 1.0f, 0.0f);

	camera->fov = 45.0f;

	camera->pitch = 0.0f;
	camera->yaw = 1.5f * 3.14;

	camera->update_forward();

	return camera;
}

glm::mat4 Camera::get_view_transform()
{
	return glm::lookAt(this->position, this->position + this->forward, this->up);
}

void Camera::move(glm::vec3 move_vector)
{
	this->position = this->position + move_vector;
}

glm::vec3 Camera::get_target()
{
	return this->position + this->forward;
}

glm::vec3 Camera::get_right()
{
	return glm::normalize(glm::cross(this->forward, this->up));
}

void Camera::update_forward()
{
	glm::vec3 camera_forward = glm::vec3(1.0f);
	camera_forward.x = cos(this->pitch) * cos(this->yaw);
	camera_forward.z = cos(this->pitch) * sin(this->yaw);
	camera_forward.y = sin(this->pitch);

	this->forward = glm::normalize(camera_forward);
}

void Camera::rotate_pitch(float amount)
{
	this->pitch += amount;
	this->update_forward();
}

void Camera::rotate_yaw(float amount)
{
	this->yaw += amount;
	this->update_forward();
}
