#version 330 core

uniform sampler2D u_scene;

in vec2 v_tc;

out vec4 f_color;

const float offset = 1.0 / 300.0;
void main(void) {
    const float gamma = 2.2;
    vec3 hdrColor = texture(u_scene, v_tc).rgb;
    vec3 mapped = vec3(1.0) - exp(-hdrColor * 0.5);
    mapped = pow(mapped, vec3(1.0));


    f_color = vec4(mapped, 1.0);
}