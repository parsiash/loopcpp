#version 330 core

in vec2 vertex_uv;

out vec4 color;

uniform sampler2D texture_0;
uniform sampler2D texture_1;

void main()
{
    color = texture(texture_0, vertex_uv) * texture(texture_1, vertex_uv);
    //color = vec4(0.4, 0.3, 0.2, 1.0);//
}