#version 330 core

out vec4 color;

uniform vec3 light_color;

void main()
{
    color = vec4(light_color, 1.0);
    //color = vec4(0.4, 0.3, 0.2, 1.0);//
}