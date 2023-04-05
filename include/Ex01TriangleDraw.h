#pragma once
#include <glad/glad.h>
#include "OGLProgram.h"

class Ex01TriangleDraw 
{
public:
    void Start();
    void Update(float InDeltaTime);
    void Destroy();

private:
    OGLProgram* Program;
    GLuint Vao;
    GLuint Vbo;
};