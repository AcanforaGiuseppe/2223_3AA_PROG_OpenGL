#version 450 core
layout (location = 0) in vec3 vert_pos;

out vec3 vert_uv_out;
uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(vert_pos, 1.f);
    vert_uv_out = vert_pos;
    //gl_Position = gl_Position.xyww; //SOL2
}
