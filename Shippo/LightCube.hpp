#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Shader.hpp"
#include "Renderable.hpp"
#include "Resources.hpp"

class LightCube : Renderable
{
public:
    LightCube(glm::vec3 position, glm::vec3 color);
    virtual void render(Shader& shader) const override;
    Shader& get_shader() {
        return shader;
    }

    void update(glm::mat4 &view, glm::mat4 &projection) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        glm::mat4 mvp = projection * view * model;
        shader.set_uniform("mvp", mvp);
        shader.set_uniform("cubeColor", light_color);
    }

    void set_position(glm::vec3 light_position) {
        position = light_position;
    }

    glm::vec3 get_position() const {
        return position;
    }

    glm::vec3 get_color() const {
        return light_color;
    }

    void set_color(glm::vec3 color) {
        light_color = color;
    }
private:
    void setup();
    const static std::vector<float> vertices;
    GLuint vao = 0, vbo = 0;
    Shader shader;

    glm::vec3 light_color;
    glm::vec3 position;
};

