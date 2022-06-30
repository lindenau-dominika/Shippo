#pragma once
#include <glad/glad.h>
#include <string>
#include <stb_image.h>

enum class TextureType {
	Albedo,
	Diffuse,
	Normal,
	Metallic
};

constexpr int get_texture_unit_index(const TextureType& texture_type) {
	switch (texture_type) {
	case TextureType::Albedo: return 6;
	case TextureType::Diffuse: return 6;
	case TextureType::Normal: return 7;
	case TextureType::Metallic: return 8;
	default: return -1;
	}
}


constexpr int get_texture_unit(const TextureType& texture_type) {
	return GL_TEXTURE0 + get_texture_unit_index(texture_type);
}


class Texture {
public:
	Texture() = default;
	Texture(Texture&& other) : data(other.data), width(other.width), height(other.height), channels(other.channels), texture_type(other.texture_type), texture_handle(other.texture_handle) {
		generate_texture();
	}
	Texture(const std::string& path, const TextureType& texture_type);
	Texture(unsigned char* texture_data, size_t size, const TextureType& texture_type);
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	~Texture();

	void load_from_file(const std::string& path);
	void load_from_memory(unsigned char* texture_data, size_t size);
	void generate_texture();

	void bind();
	int unit() const;
	int unit_index() const;

	bool is_valid();
	unsigned char* data = nullptr;
private:
	int width = 0, height = 0;
	int channels = 0;
	TextureType texture_type = TextureType::Albedo;
	GLuint texture_handle = 0;
};