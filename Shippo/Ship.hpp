#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Shader.hpp"
#include "Renderable.hpp"
#include <vector>
#include "Model.hpp"
#include "Resources.hpp"

class Ship : Renderable
{
public:
	Ship();
	Ship(glm::vec3 initial_position);
	void update(float delta_time, glm::mat4& view, glm::mat4& projection);
	virtual void render(Shader& shader) const override;

	Shader& get_shader();
	void move(glm::vec3 dir);
	glm::vec3 get_position();
	Model& get_model();

private:
	glm::vec3 position;
	glm::vec3 direction;
	float speed = 100.0f;

	Model model;
	Shader shader;
	Texture texture;
	Texture texture2;
};

