#pragma once

struct Texture
{
	unsigned char * data;
	int width;
	int height;
};

Texture * load_texture(const char * texture_path);
void free_texture(Texture * texture_data);
unsigned int generate_texture(const char * texture_path);