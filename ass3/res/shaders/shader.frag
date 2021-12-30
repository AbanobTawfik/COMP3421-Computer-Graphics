#version 330 core

in vec2 vTexCoord;

uniform sampler2D uTex;
uniform float uTexFactor;
uniform vec4 uColor;
uniform mat4 v;

out vec4 fFragColor;

in vs_out_t {
    vec3 v_normal;
    vec3 v_light_dir;
    vec3 v_view_dir;
} fs_in;

uniform vec3 u_sun_ambient;
uniform vec3 u_sun_diffuse;
uniform vec3 u_sun_specular;

uniform vec3 p_position;
uniform vec3 p_ambient;
uniform vec3 p_diffuse;
uniform vec3 p_specular;

uniform float shininess;

vec3 srgb_to_linear(vec3 color) {
    return pow(color, vec3(2.2));
}

vec4 srgb_to_linear(vec4 color) {
    return pow(color, vec4(2.2));
}

// Convert a linear color to SRGB space
vec3 linear_to_srgb(vec3 color) {
    return pow(color, vec3(1.0 / 2.2));
}

vec4 linear_to_srgb(vec4 color) {
    return pow(color, vec4(1.0 / 2.2));
}

void main() {

    vec3 n_normal = normalize(fs_in.v_normal);
    vec3 n_light_dir = normalize(fs_in.v_light_dir);
    vec3 n_view_dir = normalize(fs_in.v_view_dir);
    vec3 halfway = normalize(n_light_dir + n_view_dir);

    vec3 view_light_pos = (v*vec4(p_position, 1)).xyz;
    float light_dist = length(view_light_pos - p_position);
    float attenuation = 1.0 / (3.5 + light_dist);

    vec3 reflection = reflect(-n_light_dir, n_normal);
    vec3 H = normalize(n_light_dir + n_view_dir);


    vec3 s_diff_col = srgb_to_linear(u_sun_diffuse);
    s_diff_col += srgb_to_linear(mix(uColor, texture(uTex, vTexCoord), uTexFactor)).rgb;
    vec3 s_spec_col = srgb_to_linear(u_sun_specular);
    vec3 s_amb_col = srgb_to_linear(u_sun_ambient);

    vec3 p_diff_col = srgb_to_linear(p_diffuse);
    vec3 p_specular_col = srgb_to_linear(p_specular);
    vec3 p_ambient_col = srgb_to_linear(p_ambient);

    vec3 ambient = s_diff_col.rgb * s_amb_col;
    ambient += p_ambient_col*attenuation;

    vec3 diffuse = (s_diff_col.rgb) * max(dot(n_normal, n_light_dir), 0) * s_amb_col.rgb;
    diffuse += (p_diff_col)*attenuation;

    vec3 specular = s_spec_col.rgb * pow(max(dot(n_normal, halfway), 0), shininess) * s_spec_col.rgb;
    specular += p_specular_col*attenuation;
    fFragColor = vec4(linear_to_srgb(ambient + diffuse + specular), 1);
}

//    vec3 normal = normalize(fs_in.v_normal);
//    vec3 view_light_pos = (v * vec4(p_position, 1)).xyz;
//    vec3 light_dir = normalize(p_position - fs_in.v_light_dir);
//    float light_dist = length(p_position - fs_in.v_view_dir);
//    vec3 view_dir = normalize(-fs_in.v_view_dir);
//    vec3 reflection = reflect(-light_dir, normal);
//    vec3 halfway = normalize(light_dir + view_dir);
//
//    vec3 p_diff_col = srgb_to_linear(p_diffuse * texture(uTex, vTexCoord).rgb);
//    vec3 p_spec_col = srgb_to_linear(p_specular);
//    vec3 s_diff_col = srgb_to_linear(u_sun_diffuse);
//    vec3 s_spec_col = srgb_to_linear(s_spec_col);
//
//    vec3 light_col = srgb_to_linear(vec3(0,0,0));
//
//    vec3 diffuse = diff_col * max(dot(normal, light_dir), 0) * light_col;
//
//    vec3 specular = spec_col * pow(max(dot(normal, halfway), 0), shininess) * light_col;
//
//    vec3 color_linear = vec3(0, 0, 0);
//
//    // calculate point light contribution
//    float attenuation = 1.0 / (1.0 + 3.6 * light_dist);
//    color_linear += (diffuse + specular) * attenuation;
//
//    // linear -> sRGB conversion
//    fFragColor = vec4(linear_to_srgb(color_linear), 1.0);
