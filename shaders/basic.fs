#version 330 core


in vec3 vertex_normal;
in vec3 world_pos;

out vec4 color;

uniform vec4 main_color;


uniform vec4 light_color;
uniform vec3 light_pos;

// uniform sampler2D texture_0;
// uniform sampler2D texture_1;

void main()
{
    // color = texture(texture_0, vertex_uv) * light_color;
    //color = vec4(0.4, 0.3, 0.2, 1.0);//

    float ambient_strength = 0.1f;
    vec4 ambient = ambient_strength * light_color;

    vec3 normal = normalize(vertex_normal);
    vec3 light_dir = normalize(light_pos - world_pos);

    float diff = max(dot(normal, light_dir), 0.0);
    vec4 diffuse = diff * light_color;

    color = main_color * (ambient + diffuse);
}