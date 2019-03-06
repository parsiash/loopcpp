#include<Texture.h>
#include<glad/glad.h>
#include<stb_image.h>

Texture * load_texture(const char * texture_path)
{
	Texture * texture_data = (struct Texture *) malloc(sizeof(struct Texture));

	int channel_count;
	stbi_set_flip_vertically_on_load(true);
	texture_data->data = stbi_load(texture_path, &texture_data->width, &texture_data->height, &channel_count, 0);

	return texture_data;
}

void free_texture(Texture * texture_data)
{
	stbi_image_free(texture_data->data);
	//free(texture_data);
}

unsigned int generate_texture(const char * texture_path)
{
	Texture * texture_data = load_texture(texture_path);

	unsigned int texture_id;
	glGenTextures(1, &texture_id);

	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_data->width, texture_data->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data->data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	free_texture(texture_data);

	return texture_id;
}
