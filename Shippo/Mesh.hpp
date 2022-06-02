#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include "Shader.hpp"
#include "Renderable.hpp"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coords;
};

class Mesh : Renderable {
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

	virtual void render(Shader& shader) const override;

private:
	GLuint vao, vbo, ebo;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	void setup();
};