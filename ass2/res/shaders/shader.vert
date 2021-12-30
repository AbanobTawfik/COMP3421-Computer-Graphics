#version 330 core

uniform mat4 p;
uniform mat4 v;
uniform mat4 m;

uniform vec3 sun_pos;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoord;

out vs_out_t {
    vec3 v_normal;
    vec3 v_light_dir;
    vec3 v_view_dir;
} vs_out;

out vec2 vTexCoord;


void main() {
    mat4 mv = v * m;

    vec4 P = mv * vec4(pos, 1);

    vs_out.v_normal = mat3(mv) * normal;
    vs_out.v_light_dir = (mat3(v) * sun_pos) - P.xyz;
    vs_out.v_view_dir = -P.xyz;

    vTexCoord = aTexCoord;
    gl_Position = p * P;
}
