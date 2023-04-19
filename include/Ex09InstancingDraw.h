#pragma once
#include <glad/glad.h>
#include "OGLProgram.h"
#include "OGLTexture.h"
#include <glm/glm.hpp>
#include <vector>

struct QuadInstance 
{
    glm::vec3 Position;
};

class Ex09InstancingDraw 
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
    OGLTexture* SmileText;
    OGLTexture* WoodText;
    glm::mat4 View;
    glm::mat4 Projection;
    QuadInstance Quads[10];
    GLuint VboMvp;
    std::vector<glm::mat4> MvpData;
};