#version 330 core

uniform sampler2D u_scene;

in vec2 v_tc;

out vec4 f_color;

void main(void) {
    f_color = vec4(vec3(1.0 - texture(u_scene, v_tc)), 1.0);
}
