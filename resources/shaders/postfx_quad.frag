#version 450 core
out vec4 frag_color;

in vec2 uv_out;

layout (binding = 0) uniform sampler2D scene_tex;
layout (binding = 1) uniform sampler2D mask_tex;

uniform float time;
uniform vec2 mouse;

//TODO: Pass Screen Width and Heigh as uniform
vec4 blur() {

    vec2 textSize = textureSize(scene_tex, 0);
    float uv_w = 1.f / textSize.x;
    float uv_h = 1.f / textSize.y;

    vec4 contrib = vec4(0.f);
    for(float i=-1.f; i <= 1.f; ++i) 
    {
        float xoff = uv_w * i;
        for(float j=-1.f; j <= 1.f; ++j) 
        {
            float yoff = uv_h * j;
            vec2 calc_uv = vec2(uv_out.x + xoff, uv_out.y + yoff);
            contrib += texture(scene_tex, calc_uv);
        }        
    }

    vec4 color = contrib / 9;
    color.a = 1.f;
    return color;
}

vec4 wave() {
    //y = A sin(B(x + C)) + D
    float A = 1.f / 80.f;
    float B = 20.f;
    float C = time / 30.f;
    float D = 0;
    vec2 uv_curr = uv_out;
    uv_curr.x += A * sin(B * (uv_curr.y + C)) + D;
    return texture(scene_tex, uv_curr);
}

vec4 quake() {
    vec2 uv_new = uv_out;
    float strength = 0.01f;

    uv_new.x += cos(time * 30) * strength;
    uv_new.y += cos(time * 45) * strength;

    return texture(scene_tex, uv_new);
}

vec2 mouse_to_uv() {
    vec2 muv;
    muv.x = mouse.x / 600.f;
    muv.y = 1.f - (mouse.y / 400.f);
    return muv;
}

vec4 mask_v1() {
    //vec2 center = vec2(0.5);  //TODO: Mouse Position
    vec2 center = mouse_to_uv();
    //if (center.x < 0 || center.y < 0) return vec4(0);

    float max_ray_length = 0.1f;
    float aspect_ratio = 600.f / 400.f;

    vec2 diff = uv_out - center;
    diff.x *= aspect_ratio;
    float dist = length(diff);


    if (dist < max_ray_length) {
        return texture(scene_tex, uv_out);
    } else {
        return vec4(0);
    }
}


vec4 mask_v2() {
    vec2 center = mouse_to_uv();

    float max_radius_length = 0.2f;
    float aspect_ratio = 600.f / 400.f;

    vec2 diff = uv_out - center;
    diff.x *= aspect_ratio;
    float dist = length(diff);

    float maskValue = dist / max_radius_length;
    //mask_value = 1;
    vec2 mask_dir = normalize(diff) * maskValue; // [-1, 1]
    vec2 mask_uv = (mask_dir + vec2(1.f)) * vec2(0.5f); // [0, 1] 
    //vec2 mask_uv += vec2(0.5f);    // [0, 1]

    vec4 base = texture(scene_tex, uv_out);
    vec4 mask = texture(mask_tex, mask_uv);
    return base * mask;
}

vec4 mask_v3() {
    vec2 center = mouse_to_uv();

    float max_radius_length = 0.2f;
    float aspect_ratio = 600.f / 400.f;

    vec2 diff = uv_out - center;
    diff.x *= aspect_ratio;
    float dist = length(diff);

    float maskValue =  dist / max_radius_length;
    //mask_value = 1;
    vec2 mask_dir = normalize(diff) * maskValue; // [-1, 1]
    //vec2 mask_uv = (mask_dir + vec2(1.f)) * vec2(0.5f); // [0, 1] 
    //Range conversion could be avoided in this case, and just offsetting by 0.5 will do the job
    vec2 mask_uv = mask_dir + vec2(0.5f);    
    vec4 mask = texture(mask_tex, mask_uv);
    return mask;
}

void main() 
{
    //AS IS
    //frag_color = texture(scene_tex, uv_out);

    /* Grayscale base
    vec4 color = texture(scene_tex, uv_out);
    float avg = (color.r + color.g + color.b) / 3.0;
    frag_color = vec4(avg, avg, avg, color.a);
    */ 
    
    /* Graysale weighted 
    vec4 color = texture(scene_tex, uv_out);
    float avgw = (0.3f * color.r + 0.59f * color.g + 0.11f * color.b);
    frag_color = vec4(avgw, avgw, avgw, color.a);
    */

    /* NEGATIVE 
    vec4 color = texture(scene_tex, uv_out);
    frag_color = 1.f - color;
    */

    /* BLACKBAND with gl_FragCoord
    if (gl_FragCoord.y < 30 || gl_FragCoord.y > 370) {
        frag_color = vec4(0);
    } else {
        frag_color = texture(scene_tex, uv_out);
    }
    */

    /* BLACKBAND with uv  
    if (uv_out.y < 0.1 || uv_out.y > 0.9) {
        frag_color = vec4(0);
    } else {
        frag_color = texture(scene_tex, uv_out);
    }
    */

    /* BLUR 
    frag_color = blur();
    */

    /* WAVE 
    frag_color = wave();
    */
    
    /* QUAKE 
    frag_color = quake();
    */

    //frag_color = mask_v1();
    frag_color = mask_v2();

    //Example chainging postfx
    //frag_color = quake() * mask_v3();
}