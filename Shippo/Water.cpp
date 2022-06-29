#include "Water.hpp"
#include "Resources.hpp"

Water::Water(int grid_size) : shader(Shader::from_file(Resources::WATER_VERTEX_SHADER_PATH, Resources::WATER_FRAGMENT_SHADER_PATH)) {
	create_water_mesh(grid_size);
	setup_render();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-grid_size / 2, 0, -grid_size / 2));

	vertices.reserve(grid_size * grid_size * 6);
}

void Water::update(float delta_time, glm::mat4& view, glm::mat4& projection)
{
	total_time += delta_time;

	glm::mat4 mvp = projection * view * model;
	get_shader().set_uniform("mvp", mvp);
	get_shader().set_uniform("model", model);
	get_shader().set_uniform("waterColor", water_color);
	get_shader().set_uniform("time", total_time);
}

void Water::render(Shader& shader) const
{
	shader.use();

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() * 3);
	glBindVertexArray(0);
}

void Water::create_water_mesh(int grid_size)
{
	for (int i = 0; i < grid_size; i++) {
		for (int j = 0; j < grid_size; j++) {
			for (auto vertex : get_tile_vertices(i, j)) {
				vertices.push_back(vertex);
			}
		}
	}
}

std::vector<glm::vec3> Water::get_tile_corners(int row, int col)
{
	return {
		{(float)row, 0, (float)col},
		{(float)row, 0, (float)col + 1},
		{(float)row + 1, 0, (float)col},
		{(float)row + 1, 0, (float)col + 1},
	};
}

std::vector<glm::vec3> Water::get_tile_vertices(int row, int col)
{
	auto corner = get_tile_corners(row, col);
	return {
		corner[0], corner[1], corner[2],
		corner[1], corner[2], corner[3],
	};
}

void Water::setup_render()
{
	// Generate buffers for our mesh
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &ebo);

	// Bind vertex array for our mesh
	glBindVertexArray(vao);

	// Send vertex data to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	// Define position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3), (void*)0);

	// Unbind vertex array
	glBindVertexArray(0);
}