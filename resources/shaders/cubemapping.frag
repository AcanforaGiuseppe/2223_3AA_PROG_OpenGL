#version 450 core
out vec4 frag_color;

layout (binding = 0) uniform samplerCube sky_tex; 

in vec3 vert_uv_out;

void main() 
{
    frag_color = texture(sky_tex, vert_uv_out);
}