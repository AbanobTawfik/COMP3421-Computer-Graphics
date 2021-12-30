#version 330 core

uniform sampler2D u_scene;

in vec2 v_tc;

out vec4 f_color;

void main(void) {

    f_color = texture(u_scene, v_tc);
    float average = 0.2126 * f_color.r + 0.7152 * f_color.g + 0.0722 * f_color.b;
    f_color = vec4(average, average, average, 1.0);
}
