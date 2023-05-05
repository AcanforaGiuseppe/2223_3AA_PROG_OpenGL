#pragma once
#include <glad/glad.h>
#include "OGLProgram.h"
#include "OGLTexture.h"
#include "OGLWindow.h"

class Ex11PostFX 
{
public:
    Ex11PostFX(const OGLWindow& InWin) : Win(InWin) { }
    void Start();
    void Update(float InDeltaTime);
    void Destroy();

private:
    OGLProgram* Program;
    GLuint Vao;
    GLuint Vbo;
    OGLTexture* WoodText;

    GLuint SceneFbo;
    GLuint SceneTexture;
    GLuint SceneRbo;

    GLuint QuadVao;
    GLuint QuadVbo;
    OGLProgram* QuadProgram;

    const OGLWindow& Win;
    OGLTexture* MaskText;
};