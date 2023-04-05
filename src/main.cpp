#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <format>
#include "OGLWindow.h"
#include "Ex01TriangleDraw.h"
#include "Ex02QuadDraw.h"
#include "Ex03QuadIndexDraw.h"

#ifdef _WIN32
extern "C" {
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) unsigned long AmdPowerXpressRequestHighPerformance = 0x00000001;
}
#endif

int main() {
	std::cout << "It's Working" << std::endl;

	OGLWindow Win{600, 400, "OpenGL Window"};

	Win.PrintInfo();

	float TimeElapsed = 0.f;
	
	//Ex01TriangleDraw Scene;
	//Ex02QuadDraw Scene;
	Ex03QuadIndexDraw Scene;
	Scene.Start();

	while(Win.IsOpened()) 
	{
		float DeltaTime = Win.GetDeltaTime();
		TimeElapsed += DeltaTime;
		if (TimeElapsed >= 1.f)
		{
			TimeElapsed -= 1.f;
			float Fps = 1.f / DeltaTime;
			std::string Title = std::format("OpenGL App | DeltaTime: {} - FPS: {}", DeltaTime, Fps);
			Win.SetTitle(Title);
		}

		Scene.Update(DeltaTime);

		Win.Update();
	}
	Scene.Destroy();
	return 0;	
}