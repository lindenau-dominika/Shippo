#include "Texture.hpp"

void Texture::from_file(const std::string& path)
{
	int width, height, nr_channels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nr_channels, STBI_rgb_alpha);

	glGenTextures(1, &this->texture_handle);
	glBindTexture(GL_TEXTURE_2D, this->texture_handle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);


}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, texture_handle);
}
