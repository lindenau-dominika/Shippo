#pragma once
#include <glad/glad.h>
#include <string>
#include <stb_image.h>

class Texture {
public:
	void from_file(const std::string& path);

	void bind();
private:
	GLuint texture_handle;
};