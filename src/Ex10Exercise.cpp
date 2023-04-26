#include "Ex10Exercise.h"
#include "Common.h" 
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <vector>
//#define OBJ_PARSER_IMPLEMENTATION
#include "obj-parser.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum MeshVertexDataFilter{
    POSITION = 1 << 0,
    UV = 1 << 1,
    NORMAL = 1 << 2,
    ALL = POSITION | UV | NORMAL
};

void ReadMeshVertexData(const std::string &InObjPath, MeshVertexDataFilter InFilter, std::vector<float> &OutVertices, int &OutVerticeCount){
    obj_t* mesh = obj_parser_parse(InObjPath.c_str());

    for(int i=0; i < mesh->face_count; ++i) 
    {
        obj_triangle_t* t = &(mesh->triangles[i]);
        
        for (int j = 0; j < 3; j++)
        {
            obj_vertex_t* v = (obj_vertex_t*)((size_t)t + sizeof(obj_vertex_t) * j);
            if(InFilter & MeshVertexDataFilter::POSITION){
                OutVertices.push_back(v->position.x);
                OutVertices.push_back(v->position.y);
                OutVertices.push_back(v->position.z);
            }
            if(InFilter & MeshVertexDataFilter::UV){
                OutVertices.push_back(v->uv.x);
                OutVertices.push_back(v->uv.y);
            }
            if(InFilter & MeshVertexDataFilter::NORMAL){
                OutVertices.push_back(v->normal.x);
                OutVertices.push_back(v->normal.y);
                OutVertices.push_back(v->normal.z);
            }
        }
    }
    OutVerticeCount = mesh->face_count * 3;
    obj_parser_free(mesh);
}


void Ex10Exercise::Start()
{
    //Storm Trupper scenario
    Program = new OGLProgram("resources/shaders/exercise.vert", "resources/shaders/exercise.frag");
    
    std::vector<float> Vertices;
    ReadMeshVertexData("resources/models/stormtrooper.obj", MeshVertexDataFilter::ALL, Vertices, VerticeCount);

    //1. Create VAO
    glGenVertexArrays(1, &Vao);
    glBindVertexArray(Vao);

    //2. Create VBO to load data
    glGenBuffers(1, &Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, Vbo);

    int DataSize = Vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, DataSize, Vertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    GLuint Location_0 = 0;
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    GLuint Location_2 = 2;
    glVertexAttribPointer(Location_2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(Location_2);

    //4. Set Viewport
    glViewport(0, 0, 600, 400);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CCW); //default
    //glCullFace(GL_BACK); //default

    //camera
    glm::vec3 Position = glm::vec3(0, 0, 8);
    glm::vec3 Direction = glm::vec3(0, 0, -1);
    glm::vec3 Up = glm::vec3(0, 1, 0);
    float FovY = 60;
    float AspectRatio = 600.f / 400.f;
    float ZNear = 0.01f;
    float ZFar = 1000.f;

    View = glm::lookAt(Position, Position + Direction, Up);
    Projection = glm::perspective(glm::radians(FovY), AspectRatio, ZNear, ZFar);


    PointLightPos = glm::vec3(1, 0, 0);
    Program->Bind();
    Program->SetUniform("point_light_pos", PointLightPos);
    Program->SetUniform("camera_pos", Position);
    StormText = new OGLTexture("resources/models/stormtrooper.png");
    StormText->Bind(GL_TEXTURE0);


    //Light scenario
    CubeProgram = new OGLProgram("resources/shaders/exercise_light.vert", "resources/shaders/exercise_light.frag");
    std::vector<float> CubeVertices = {
        //FRONT FACE
        //positions      uvs
        -1, -1, 1,   0, 0,    //bottom-left
         1, -1, 1,   1, 0,    //bottom-right
         1,  1, 1,   1, 1,    //top-right
        -1,  1, 1,    0, 1,    //top-left 
        -1, -1, 1,   0, 0,    //bottom-left
         1,  1, 1,   1, 1,    //top-right

        //BACK FACE
         1, -1,-1,   0, 0,    //bottom-left
        -1, -1,-1,   1, 0,    //bottom-right
        -1,  1,-1,   1, 1,    //top-right
         1,  1,-1,   0, 1,    //top-left 
         1, -1,-1,   0, 0,    //bottom-left
        -1,  1,-1,   1, 1,    //top-right

         //LEFT FACE
        -1, -1,-1,   0, 0,    //bottom-left
        -1, -1, 1,   1, 0,    //bottom-right
        -1,  1, 1,   1, 1,    //top-right
        -1,  1,-1,   0, 1,    //top-left 
        -1, -1,-1,   0, 0,    //bottom-left
        -1,  1, 1,   1, 1,    //top-right

        //RIGHT FACE
         1, -1, 1,   0, 0,    //bottom-left
         1, -1,-1,   1, 0,    //bottom-right
         1,  1,-1,   1, 1,    //top-right
         1,  1, 1,   0, 1,    //top-left 
         1, -1, 1,   0, 0,    //bottom-left
         1,  1,-1,   1, 1,    //top-right

         //TOP FACE
         -1, 1, 1,   0, 0,    //bottom-left
          1, 1, 1,   1, 0,    //bottom-right
          1, 1,-1,   1, 1,    //top-right
         -1, 1,-1,   0, 1,    //top-left 
         -1, 1, 1,   0, 0,    //bottom-left
          1, 1,-1,   1, 1,    //top-right
         
         //BOTTOM FACE
         -1,-1,-1,   0, 0,    //bottom-left
          1,-1,-1,   1, 0,    //bottom-right
          1,-1, 1,   1, 1,    //top-right
         -1,-1, 1,   0, 1,    //top-left 
         -1,-1,-1,   0, 0,    //bottom-left
          1,-1, 1,   1, 1,    //top-right
    };


    CubeVerticeCount = CubeVertices.size() / 5;

    //1. Create VAO
    glGenVertexArrays(1, &CubeVao);
    glBindVertexArray(CubeVao);
    
    //2. Create VBO to load data
    glGenBuffers(1, &CubeVbo);
    glBindBuffer(GL_ARRAY_BUFFER, CubeVbo);

    int CubeDataSize = CubeVertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, CubeDataSize, CubeVertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //CubeProgram->Bind();
    //CubeProgram->SetUniform("color", Color{1,1,1,1});

    //Suzanne Scenario
    std::vector<float> SuzanneVertices;
    ReadMeshVertexData("resources/models/suzanne.obj", MeshVertexDataFilter::POSITION, SuzanneVertices, SuzanneVerticeCount);
    //1. Create VAO
    glGenVertexArrays(1, &SuzanneVao);
    glBindVertexArray(SuzanneVao);
    
    //2. Create VBO to load data
    glGenBuffers(1, &SuzanneVbo);
    glBindBuffer(GL_ARRAY_BUFFER, SuzanneVbo);

    int SuzanneDataSize = SuzanneVertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, SuzanneDataSize, SuzanneVertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}

void Ex10Exercise::Update(float InDeltaTime)
{
    glm::vec3 StormTrupperPos = glm::vec3(3, -4, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;
    float Angle = 20.f * ElapsedTime;

    Program->Bind();
    glBindVertexArray(Vao);

    //Model =  translate * rotate * scale;
    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, StormTrupperPos);
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
    Model = glm::scale(Model, glm::vec3(2.f));


    glm::mat4 Mvp = Projection * View * Model;
    Program->SetUniform("mvp", Mvp);
    Program->SetUniform("model", Model);
    glDrawArrays(GL_TRIANGLES, 0, VerticeCount);

    {
        CubeProgram->Bind();
        glBindVertexArray(CubeVao);
    
        glm::mat4 CubeModel = glm::mat4(1.f);
        CubeModel = glm::translate(CubeModel, PointLightPos + StormTrupperPos + glm::vec3(0,4,0)); //vec3(1, 0, 0) + vec3(3, -4, 0) + pivot cancelling
        CubeModel = glm::scale(CubeModel, glm::vec3(0.2f));

        glm::mat4 T1 = glm::translate(glm::mat4(1.f), StormTrupperPos);
        glm::mat4 Rotation = glm::rotate(glm::mat4(1.f), glm::radians(Angle), glm::vec3(0, 1, 0));
        glm::mat4 T2 = glm::translate(glm::mat4(1.f), -StormTrupperPos);
        glm::mat4 RotationAround = T1 * Rotation * T2;


        glm::mat4 CubeMvp = Projection * View * RotationAround * CubeModel;
        CubeProgram->SetUniform("mvp", CubeMvp);
        CubeProgram->SetUniform("color", Color{1,1,1,1});
        glDrawArrays(GL_TRIANGLES, 0, CubeVerticeCount);
    }

    {
        glBindVertexArray(SuzanneVao);
        int nSuzanne = 5;
        float deltaAngle = 360 / nSuzanne;
        for (int i = 0; i < nSuzanne; i++)
        {
            float offsetAngle = deltaAngle * i;
            float x = cos(glm::radians(offsetAngle));
            float z = sin(glm::radians(offsetAngle));
            glm::vec3 dir = glm::vec3(x,0,z);
            glm::mat4 SuzanneModel = glm::mat4(1.f);
            SuzanneModel = glm::translate(SuzanneModel, StormTrupperPos + dir * glm::vec3(2) + glm::vec3(0,7,0));

            glm::mat4 lookAtResult = glm::lookAt(glm::vec3(0), dir, glm::vec3(0,1,0));
            SuzanneModel = glm::rotate(SuzanneModel, glm::radians(180.f), glm::vec3(0,1,0));
            SuzanneModel *= glm::inverse(lookAtResult);
            SuzanneModel = glm::scale(SuzanneModel, glm::vec3(0.5f));
            glm::mat4 SuzanneMvp = Projection * View * SuzanneModel;
            CubeProgram->SetUniform("mvp", SuzanneMvp);
            CubeProgram->SetUniform("color", Color{1,1,0.5,1});
            glDrawArrays(GL_TRIANGLES, 0, SuzanneVerticeCount);
        }
        
    }

}

void Ex10Exercise::Destroy()
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteVertexArrays(1, &CubeVao);
    glDeleteBuffers(1, &Vbo);
    glDeleteBuffers(1, &CubeVbo);
    delete Program;
    delete CubeProgram;
    delete StormText;
}