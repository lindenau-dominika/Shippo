#pragma once
#include <glad/glad.h>
#include <string>
#include <stb_image.h>

enum class TextureType {
	Albedo,
	Normal,
	Metallic
};

constexpr int get_texture_unit_index(const TextureType& texture_type) {
	switch (texture_type) {
	case TextureType::Albedo: return 6;
	case TextureType::Normal: return 7;
	case TextureType::Metallic: return 8;
	default: return -1;
	}
}


constexpr int get_texture_unit(const TextureType& texture_type) {
	switch (texture_type) {
	case TextureType::Albedo: return GL_TEXTURE0 + get_texture_unit_index(texture_type);
	case TextureType::Normal: return GL_TEXTURE0 + get_texture_unit_index(texture_type);
	case TextureType::Metallic: return GL_TEXTURE0 + get_texture_unit_index(texture_type);
	default: return -1;
	}
}


class Texture {
public:
	Texture(const std::string& path, const TextureType& texture_type);
	~Texture();

	void load_from_file(const std::string& path);
	void generate_texture();

	void bind();
	int unit() const;
	int unit_index() const;
private:

	unsigned char* data;
	int width, height;
	int channels;
	TextureType texture_type;
	GLuint texture_handle;
};