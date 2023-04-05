#pragma once
#include <string>
#include <glad/glad.h>

class OGLProgram 
{
public:
    OGLProgram(const std::string& InVertShaderPath, const std::string& InFragShaderPath);
    ~OGLProgram();
    void Bind();
    GLuint ID();
private:
    GLuint ProgramId;
};