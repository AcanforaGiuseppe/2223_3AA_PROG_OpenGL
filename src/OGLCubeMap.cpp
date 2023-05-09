#include "OGLCubeMap.h"
#include "Common.h"
#include "stb_image.h"
#include <vector>

static GLuint CreateTexture(const std::vector<std::string>& InPaths) 
{
    //stbi_set_flip_vertically_on_load(true);
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    //GL_TEXTURE_CUBE_MAP_POSITIVE_X //Right

    for(int i=0; i < 6; i++)
    {
        const std::string& path  = InPaths[i];

        int Width, Height, Channels;
        unsigned char* Data = stbi_load(path.c_str(), &Width, &Height, &Channels, 0);
        DIE_ON_NULL(Data, "Failed Loading Texture!");

        GLenum Format = Channels == 3 ? GL_RGB : GL_RGBA;

        // Load Data to GPU
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);

        stbi_image_free(Data);
    }

    // Wrapping
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Filtering
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    return textureId;
}

OGLCubeMap::OGLCubeMap(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& front, const std::string& back)
{
    std::vector<std::string> paths {right, left, top, bottom, front, back };
    Id = CreateTexture(paths);
}

OGLCubeMap::~OGLCubeMap() 
{
    glDeleteTextures(1, &Id);
}

void OGLCubeMap::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Id);
}