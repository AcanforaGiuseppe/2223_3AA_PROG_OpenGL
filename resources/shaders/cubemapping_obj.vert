#version 450 core
layout (location = 0) in vec3 vert_pos;

out vec3 color;
uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(vert_pos, 1.f);
    color = vert_pos;
}