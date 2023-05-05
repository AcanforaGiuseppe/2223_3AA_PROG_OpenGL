#pragma once
#include <glad/glad.h>
#include "OGLProgram.h"
#include "OGLCubeMap.h"
#include <glm/glm.hpp>

class Ex13CubeMappingDraw 
{
public:
    void Start();
    void Update(float InDeltaTime);
    void Destroy();

private:
    OGLProgram* Program;
    GLuint Vao;
    GLuint Vbo;
    GLuint Ebo;
    OGLCubeMap* SkyText;
    glm::mat4 View;
    glm::mat4 Projection;
    OGLProgram* ObjProgram;
};