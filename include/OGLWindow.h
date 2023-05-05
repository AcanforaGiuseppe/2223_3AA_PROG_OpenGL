#pragma once
#include <string>
#include <glm/glm.hpp>

typedef struct GLFWwindow GLFWwindow;

class OGLWindow 
{
public:
    OGLWindow(int InWidth, int InHeight, const std::string& InTitle);
    ~OGLWindow();
    
    void PrintInfo();
    bool IsOpened();
    float GetDeltaTime();
    void Update();
    void SetTitle(const std::string& InTitle);
    glm::vec2 MousePosition() const;

private:
    int Width;
    int Height;
    std::string Title;
    GLFWwindow* RawWindow;
    float DeltaTime;
    float LastTime;
};