#version 450 core
out vec4 frag_color;

layout (binding = 0) uniform sampler2D smile_tex; 
layout (binding = 1) uniform sampler2D wood_tex;

in vec2 vert_uv_out;
in flat int inst_index;

void main() 
{
    vec4 smile_texel = texture(smile_tex, vert_uv_out);
    vec4 wood_texel = texture(wood_tex, vert_uv_out);

/*
    if (inst_index % 2 == 0)
        frag_color = smile_texel;
    else
        frag_color = wood_texel;
*/
    // eventually with function: mod(inst_index, 2);
    frag_color = mix(smile_texel, wood_texel, inst_index % 2);
}