#include "Texture.hpp"
#include <iostream>

Texture::Texture(const std::string& path, const TextureType& texture_type) : texture_type(texture_type)
{
	load_from_file(path);
	generate_texture();
}

Texture::Texture(unsigned char* texture_data, size_t size, const TextureType& texture_type) : texture_type(texture_type)
{
	load_from_memory(texture_data, size);
	generate_texture();
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture_handle);
}

void Texture::load_from_file(const std::string& path)
{
	data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if (data == nullptr) {
		std::cerr << "Failed to load texture " << path << std::endl;
		return;
	}
}

void Texture::load_from_memory(unsigned char* texture_data, size_t size)
{
	data = stbi_load_from_memory(texture_data, size, &width, &height, &channels, STBI_rgb_alpha);
	if (data == nullptr) {
		std::cerr << "Failed to load texture from memory " << (size_t)texture_data << std::endl;
		return;
	}
}

void Texture::generate_texture()
{
	glGenTextures(1, &this->texture_handle);
	glActiveTexture(unit());
	glBindTexture(GL_TEXTURE_2D, this->texture_handle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::bind()
{
	glActiveTexture(unit());
	glBindTexture(GL_TEXTURE_2D, texture_handle);
}

int Texture::unit() const
{
	return get_texture_unit(texture_type);
}

int Texture::unit_index() const
{
	return get_texture_unit_index(texture_type);
}

bool Texture::is_valid()
{
	return data != nullptr;
}

