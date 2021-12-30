#version 330 core

uniform sampler2D tex;
in vec2 v_tex_coords;
out vec4 fs_color;

void main() {
//    fs_color = vec4(0, 0, 0, 1);
    fs_color = texture(tex, v_tex_coords);
}