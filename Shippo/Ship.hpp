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
	void print_textures();

	void on(float delta_time, int dir) {
		speed = delta_time * dir * 1000.0f;
	}

	void off() {
		speed = 0.0f;
	}

	void turn(float delta_time, int dir) {
		yaw += dir * turning_speed * delta_time;
	}

private:
	glm::vec3 position;
	glm::vec3 direction;
	float yaw = 0;
	float speed = 0.0f;
	float turning_speed = 3.14 / 2; // To w radianach k¹t

	Model model;
	Shader shader;
	Texture texture;
	Texture texture2;
};

