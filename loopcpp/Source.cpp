#include<iostream>
#include <string>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<Engine.h>

using namespace std;

void resize_callback(GLFWwindow * window, int width, int height);

int main()
{
	//initialize glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create glfw window
	GLFWmonitor * monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode * video_mode = glfwGetVideoMode(monitor);
	int screen_width = video_mode->width;
	int screen_height = video_mode->height;
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	//GLFWwindow * window = glfwCreateWindow(screen_width / 2, screen_height / 2, "Parsiash", NULL, NULL);
	if (window == NULL)
	{
		cout << "failed to create the glfw window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	//set viewport
	glfwSetFramebufferSizeCallback(window, resize_callback);
	
	//initialize glad to load opengl functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "failed to load opengl functions" << endl;
		return -1;
	}

	cout << "window created" << endl;

		
	//intialize the enginep
	struct Engine * engine = create_engine(window, 800, 600);

	cout << "engine initialized" << endl;

	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//update engine
		engine->update();

		//update glfw
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glDeleteBuffers(1, &engine->vbo);
	glDeleteBuffers(1, &engine->ebo);
	glDeleteVertexArrays(1, &engine->test_vao);
	glfwTerminate();
	cout << "terminated" << endl;
	return 0;
}

void resize_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}