#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include "Shader.hpp"
#include "Renderable.hpp"
#include "Texture.hpp"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coords;
};

class Mesh : Renderable {
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::string texture_name);

	virtual void render(Shader& shader) const override;
	void apply_texture(Texture* texture);
	void apply_texture_uniform_name(std::string name) {
		texture_uniform_name = name;
	}
	std::string get_texture_name() {
		return texture_name;
	}

private:
	GLuint vao, vbo, ebo;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Texture* texture = nullptr;
	std::string texture_name;
	std::string texture_uniform_name;

	void setup();
};