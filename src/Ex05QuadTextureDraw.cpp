#include "Ex05QuadTextureDraw.h"
#include "Common.h" 
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <vector>

void Ex05QuadTextureDraw::Start()
{
    Program = new OGLProgram("resources/shaders/quadtexture.vert", "resources/shaders/quadtexture.frag");
    
    std::vector<float> Vertices =
    {
        // Positions        // Uvs
        -0.5f,  0.5f, 0.f,  0.f, 1.f, // top left
        -0.5f, -0.5f, 0.f,  0.f, 0.f, // bottom left
         0.5f,  0.5f, 0.f,  1.f, 1.f, // top right    
         0.5f, -0.5f, 0.f,  1.f, 0.f  // bottom right
    };

    std::vector<uint32_t> Indexes =
    {
        0, 1, 2, // Left Triangle
        2, 3, 1, // Right Triangle
    };

    // 1. Create VAO
    glGenVertexArrays(1, &Vao);
    glBindVertexArray(Vao);

    // 2. Create VBO to load data
    glGenBuffers(1, &Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, Vbo);

    int DataSize = Vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, DataSize, Vertices.data(), GL_STATIC_DRAW);

    // 3. Link to Vertex Shader
    GLuint Location_0 = 0;
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    // 4. Crea EBO
    glGenBuffers(1, &Ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
    int EboSize = Indexes.size() * sizeof(uint32_t);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, EboSize, Indexes.data(), GL_STATIC_DRAW);

    // 4. Set Viewport
    glViewport(0, 0, 600, 400);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind();

/*
    GLuint SmileTextureId = CreateTexture("resources/textures/smile.png");
    GLuint WoodBoxTextureId = CreateTexture("resources/textures/wood-box.jpg");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, SmileTextureId);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, WoodBoxTextureId);
*/

    SmileText = new OGLTexture("resources/textures/smile.png");
    WoodText = new OGLTexture("resources/textures/wood-box.jpg");

    SmileText->Bind(GL_TEXTURE0);
    WoodText->Bind(GL_TEXTURE1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Ex05QuadTextureDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;

    Color TimeColor;
    TimeColor.R = sinf(ElapsedTime) * 0.5f + 0.5f;
    TimeColor.G = cosf(ElapsedTime) * 0.5f + 0.5f;
    TimeColor.B = cosf(ElapsedTime + 1.1f) * 0.5f + 0.5f;
    TimeColor.A = 1.f;

    //glUniform4fv(glGetUniformLocation(Program->ID(), "base_color"), 1, (GLfloat*)&TimeColor);
    Program->SetUniform("base_color", TimeColor);
}

void Ex05QuadTextureDraw::Destroy()
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    glDeleteBuffers(1, &Ebo);
    delete Program;
    delete SmileText;
    delete WoodText;
}