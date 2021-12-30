#version 330 core

layout (location = 0) in vec4 a_pos;
layout (location = 1) in vec2 a_tex_coords;

out vec2 v_tex_coords;

uniform mat3 u_matrix;

void main() {
    v_tex_coords = a_tex_coords;
    vec3 pos3d = u_matrix*vec3(a_pos.x, a_pos.y, 1);
    gl_Position = vec4(pos3d, 1);
}