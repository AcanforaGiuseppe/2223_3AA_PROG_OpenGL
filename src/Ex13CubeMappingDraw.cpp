#include "Ex13CubeMappingDraw.h"
#include "Common.h" 
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

void Ex13CubeMappingDraw::Start()
{
    Program = new OGLProgram("resources/shaders/cubemapping.vert", "resources/shaders/cubemapping.frag");
    
    std::vector<float> Vertices =
    {
        // positions     
        // FRONT FACE
        -1, -1, 1,     //bottom-left
         1, -1, 1,     //bottom-right
         1,  1, 1,     //top-right
        -1,  1, 1,     //top-left 
        -1, -1, 1,     //bottom-left
         1,  1, 1,     //top-right

        // BACK FACE
         1, -1,-1,  //bottom-left
        -1, -1,-1,  //bottom-right
        -1,  1,-1,  //top-right
         1,  1,-1,   //top-left 
         1, -1,-1,   //bottom-left
        -1,  1,-1,   //top-right

         // LEFT FACE
        -1, -1,-1,  //bottom-left
        -1, -1, 1,  //bottom-right
        -1,  1, 1,  //top-right
        -1,  1,-1,  //top-left 
        -1, -1,-1,  //bottom-left
        -1,  1, 1,  //top-right

        // RIGHT FACE
         1, -1, 1, //bottom-left
         1, -1,-1, //bottom-right
         1,  1,-1, //top-right
         1,  1, 1, //top-left 
         1, -1, 1, //bottom-left
         1,  1,-1, //top-right

         // TOP FACE
         -1, 1, 1, //bottom-left
          1, 1, 1, //bottom-right
          1, 1,-1, //top-right
         -1, 1,-1, //top-left 
         -1, 1, 1, //bottom-left
          1, 1,-1, //top-right
         
         // BOTTOM FACE
         -1,-1,-1, //bottom-left
          1,-1,-1, //bottom-right
          1,-1, 1, //top-right
         -1,-1, 1, //top-left 
         -1,-1,-1, //bottom-left
          1,-1, 1, //top-right
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
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);


    // 4. Set Viewport
    glViewport(0, 0, 600, 400);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind();

    SkyText = new OGLCubeMap(
        "resources/textures/cubemap/right.jpg",
        "resources/textures/cubemap/left.jpg",
        "resources/textures/cubemap/top.jpg",
        "resources/textures/cubemap/bottom.jpg",
        "resources/textures/cubemap/front.jpg",
        "resources/textures/cubemap/back.jpg"
        );

    // Camera
    glm::vec3 Position = glm::vec3(0, 0, 8);
    glm::vec3 Direction = glm::vec3(0, 0, -1);
    glm::vec3 Up = glm::vec3(0, 1, 0);
    float FovY = 60;
    float AspectRatio = 600.f / 400.f;
    float ZNear = 0.01f;
    float ZFar = 1000.f;

    View = glm::lookAt(Position, Position + Direction, Up);
    Projection = glm::perspective(glm::radians(FovY), AspectRatio, ZNear, ZFar);

    ObjProgram = new OGLProgram("resources/shaders/cubemapping_obj.vert", "resources/shaders/cubemapping_obj.frag");

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
}

void Ex13CubeMappingDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 /* DRAW OBJ  SOL2
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);

    glm::mat4 ObjModel = glm::mat4(1.f);
    ObjModel = glm::translate(ObjModel, glm::vec3(0, 0, -10));
    ObjModel = glm::rotate(ObjModel, glm::radians(45.f), glm::vec3(0, 1.f, 0));
    ObjModel = glm::scale(ObjModel, glm::vec3(2.f));

    glm::mat4 ObjMvp = Projection * View * ObjModel;
    ObjProgram->Bind();
    ObjProgram->SetUniform("mvp", ObjMvp);
    glDrawArrays(GL_TRIANGLES, 0, 36);
*/

    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;
    float Angle = 20.f * ElapsedTime;

    /* DRAW SKYBOX */
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);
    //glDepthFunc(GL_LEQUAL); //SOL2

    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1.f, 0));

    glm::mat4 newView = glm::mat3(View); //to avoid camera traslation
    glm::mat4 Mvp = Projection * newView * Model;
    Program->Bind();
    Program->SetUniform("mvp", Mvp);

    glDrawArrays(GL_TRIANGLES, 0, 36);

   /* DRAW OBJ */
    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);

    glm::mat4 ObjModel = glm::mat4(1.f);
    ObjModel = glm::translate(ObjModel, glm::vec3(0, 0, -10));
    ObjModel = glm::rotate(ObjModel, glm::radians(45.f), glm::vec3(0, 1.f, 0));
    ObjModel = glm::scale(ObjModel, glm::vec3(2.f));

    glm::mat4 ObjMvp = Projection * View * ObjModel;
    ObjProgram->Bind();
    ObjProgram->SetUniform("mvp", ObjMvp);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Ex13CubeMappingDraw::Destroy()
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete Program;
    delete SkyText;
    delete ObjProgram;
}