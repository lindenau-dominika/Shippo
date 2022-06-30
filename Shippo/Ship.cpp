#include "Ship.hpp"

Ship::Ship() : Ship(glm::vec3(0.0, 0.0, 0.0)) {
}

Ship::Ship(glm::vec3 initial_position) : model(Resources::SHIP_MODEL_PATH), shader(Shader::from_file(Resources::SHIP_VERTEX_SHADER_PATH, Resources::SHIP_FRAGMENT_SHADER_PATH)), position(initial_position), direction(glm::vec3(0.0))
{
}


void Ship::update(float delta_time, glm::mat4& view, glm::mat4& projection)
{
	position += delta_time * speed * direction;

	glm::mat4 model_matrix = glm::mat4(1.0f);
	model_matrix = glm::translate(model_matrix, position);
	model_matrix = glm::scale(model_matrix, glm::vec3(0.1));
	glm::mat4 mvp = projection * view * model_matrix;
	shader.use();
	shader.set_uniform("mvp", mvp);
	shader.set_uniform("model", model_matrix);

	auto& textures = model.get_textures();
	model.add_texture_uniform_name("woodenpanles", "albedoTexture");
	model.add_texture_uniform_name("metal", "albedoTexture");
	model.add_texture_uniform_name("darkwood", "albedoTexture");
}

void Ship::render(Shader& shader) const
{
	shader.use();

	model.render(shader);
}

Shader& Ship::get_shader() {
	return shader;
}

void Ship::move(glm::vec3 dir) {
	direction = dir;
}

glm::vec3 Ship::get_position() {
	return position;
}

Model& Ship::get_model() {
	return model;
}

void Ship::print_textures()
{
	for (auto& [k, v] : model.get_textures()) {
		std::cout << k << "\t";
	} std::cout << std::endl;
}
