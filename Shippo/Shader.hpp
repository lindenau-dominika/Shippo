#pragma once
#include <string_view>
#include <optional>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


enum class ShaderType {
	Vertex,
	Geometry,
	Fragment
};


class Shader {
public:
	Shader(const std::string& vertex_source, const std::string& fragment_source);
	Shader(const std::string& vertex_source, const std::string& fragment_source, const std::string& geometry_source);

	static Shader from_file(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
	static Shader from_file(const std::string& vertex_shader_path, const std::string& fragment_shader_path, const std::string& geometry_shader_path);

	void use();

	void set_uniform(const std::string& name, int value);
	void set_uniform(const std::string& name, float value);
	void set_uniform(const std::string& name, const glm::vec2& value);
	void set_uniform(const std::string& name, const glm::vec3& value);
	void set_uniform(const std::string& name, const glm::vec4& value);
	void set_uniform(const std::string& name, const glm::mat2& value);
	void set_uniform(const std::string& name, const glm::mat3& value);
	void set_uniform(const std::string& name, const glm::mat4& value);

	GLuint get_uniform_location(const std::string& name) const;
	GLuint get_program_handle() const;

private:
	GLuint program_handle;

	GLuint compile_source(const ShaderType& shader_type, const std::string& source);
	void link_program(GLuint vertex_shader_handle, std::optional<GLuint> geometry_shader_handle, GLuint fragment_shader_handle);

	constexpr static GLuint get_shader_type_code(const ShaderType& shader_type);

	static std::optional<std::string> check_shader_error(GLuint shader_handle);
	static std::optional<std::string> check_program_error(GLuint program_handle);

	static std::string read_shader_file(const std::string& path);


};
