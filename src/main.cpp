#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <format>

#ifdef _WIN32
extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 0x00000001;
}
#endif


int main() {
	std::cout << "It's Working" << std::endl;


	if (glfwInit() == 0) 
	{
		std::cout << "Init failed!" << std::endl;
		return 0;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	
	int width = 600;
	int height = 400;
	const std::string title = "HelloWindow";

	bool fullScreen = false;
	GLFWmonitor* monitor = fullScreen ? glfwGetPrimaryMonitor() : NULL;
	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), monitor, NULL);

	if (window == NULL) 
	{
		std::cout << "Window failed!" << std::endl;
		return 0;
	}

	glfwMakeContextCurrent(window);

	if (gladLoadGL() == 0) 
	{
		std::cout << "Glad Load failed!" << std::endl;
		return 0;
	}

	std::cout << "GL VERSION: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GL RENDERER: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "GL VENDOR: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "GLSL VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	float deltaTime = 0.f;
	float lastTime = glfwGetTime();

	while(!glfwWindowShouldClose(window)) 
	{
		float currTime = glfwGetTime();
		deltaTime = (currTime - lastTime);
		lastTime = currTime;

		float fps = 1.f / deltaTime;
		std::string title = std::format("OpenGL App | DeltaTime: {} - FPS: {}", deltaTime, fps);

		glfwSetWindowTitle(window, title.c_str());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	return 0;	
}