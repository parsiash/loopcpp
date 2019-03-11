#version 330 core

out vec4 color;

uniform vec4 light_color;

void main()
{
    color = light_color;
    //color = vec4(0.4, 0.3, 0.2, 1.0);//
}