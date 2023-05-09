#pragma once

#include <glad/glad.h>
#include <string>

class OGLCubeMap 
{
    public:
        OGLCubeMap(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& front, const std::string& back);
        ~OGLCubeMap();
        void Bind(GLenum TextureUnit);
        
    private:
       GLuint Id; 
};