#pragma once
#include <string>
#include <glad/glad.h>
#include "Common.h"

class OGLProgram 
{
public:
    OGLProgram(const std::string& InVertShaderPath, const std::string& InFragShaderPath);
    ~OGLProgram();
    void Bind();
    GLuint ID();
    void SetUniform(const std::string& name, const Color& color);
private:
    GLuint ProgramId;
};