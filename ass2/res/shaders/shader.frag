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


void main() {

    vec3 n_normal = normalize(fs_in.v_normal);
    vec3 n_light_dir = normalize(fs_in.v_light_dir);
    vec3 n_view_dir = normalize(fs_in.v_view_dir);

    vec3 view_light_pos = (v*vec4(p_position, 1)).xyz;
    float light_dist = length(view_light_pos - p_position);
    float attenuation = 1.0 / (3.5 + light_dist);

    vec3 reflection = reflect(-n_light_dir, n_normal);
    vec3 H = normalize(n_light_dir + n_view_dir);


    vec4 junk = mix(uColor, texture(uTex, vTexCoord), uTexFactor);
    vec4 diff_col = texture(uTex, vTexCoord);
    vec4 spec_col = vec4(0.9, 0.9, 0.9, 1.0) + junk - (junk - vec4(1.0));

    vec3 ambient = diff_col.rgb * u_sun_ambient.rgb;
    ambient += p_ambient*attenuation;
    vec3 diffuse = (diff_col.rgb) * max(dot(n_normal, n_light_dir), 0) * u_sun_diffuse.rgb;
    diffuse += (p_diffuse)*attenuation;

    vec3 specular = spec_col.rgb * pow(max(dot(n_normal, H), 0), shininess) * u_sun_specular.rgb;
//    vec3 specular = spec_col.rgb * pow(max(dot(n_view_dir, reflection), 0), shininess) * u_sun_specular.rgb;
    specular += p_specular*attenuation;
    fFragColor = vec4(ambient + diffuse + specular, diff_col.a);
//    fFragColor = mix(uColor, texture(uTex, vTexCoord), uTexFactor);
}
