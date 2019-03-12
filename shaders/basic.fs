#version 330 core


in vec3 vertex_normal;
in vec3 world_pos;

out vec4 color;

uniform vec4 main_color;


uniform vec4 light_color;
uniform vec3 light_pos;
uniform vec3 eye_pos;

// uniform sampler2D texture_0;
// uniform sampler2D texture_1;

void main()
{
    //ambient component
    float ambient_strength = 0.1;
    vec4 ambient = ambient_strength * light_color;

    //diffuse component
    vec3 normal = normalize(vertex_normal);
    vec3 light_dir = normalize(light_pos - world_pos);
    float diff = max(dot(normal, light_dir), 0.0);
    vec4 diffuse = diff * light_color;

    //specular component
    float specular_strength = 0.5;
    vec3 reflect_dir = 2.0 * dot(light_dir, normal) * normal - light_dir;
    vec3 eye_dir = normalize(eye_pos - world_pos);
    vec4 specular = pow(max(dot(eye_dir, reflect_dir), 0), 32) * light_color * specular_strength;


    color = main_color * (ambient + diffuse + specular);
}