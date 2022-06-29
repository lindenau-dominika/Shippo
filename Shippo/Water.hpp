#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Shader.hpp"
#include "Renderable.hpp"
#include <vector>

class Water : Renderable
{
public:
	Water(int grid_size);
	void update(float delta_time, glm::mat4& view, glm::mat4& projection);
	virtual void render(Shader& shader) const override;
	Shader& get_shader() {
		return shader;
	}
private:
	void create_water_mesh(int grid_size);
	std::vector<glm::vec3> get_tile_corners(int row, int col);
	std::vector<glm::vec3> get_tile_vertices(int row, int col);
	void setup_render();

	std::vector<glm::vec3> vertices;
	glm::mat4 model;
	Shader shader;

	float total_time;
	
	GLuint vao, vbo, ebo;
	glm::vec4 water_color = glm::vec4(0.0, 0.2, 1.0, 1.0);
};

