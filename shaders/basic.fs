#version 330 core


in vec3 vertex_normal;
in vec3 world_pos;

out vec4 color;

uniform vec3 main_color;


uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 eye_pos;


struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

// uniform sampler2D texture_0;
// uniform sampler2D texture_1;

void main()
{
    //ambient component
    vec3 ambient = material.ambient * light_color;

    //diffuse component
    vec3 normal = normalize(vertex_normal);
    vec3 light_dir = normalize(light_pos - world_pos);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * material.diffuse * light_color;

    //specular component
    vec3 reflect_dir = 2.0 * dot(light_dir, normal) * normal - light_dir;
    vec3 eye_dir = normalize(eye_pos - world_pos);
    vec3 specular = pow(max(dot(eye_dir, reflect_dir), 0), material.shininess) * light_color * material.specular;


    color = vec4(ambient + diffuse + specular, 1.0);
}