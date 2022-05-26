#include "Shader.hpp"

Shader::Shader(const std::string& vertex_source, const std::string& fragment_source)
{
	GLuint vertex_shader_handle = compile_source(ShaderType::Vertex, vertex_source);
	GLuint fragment_shader_handle = compile_source(ShaderType::Fragment, fragment_source);

	link_program(vertex_shader_handle, {}, fragment_shader_handle);
}

Shader::Shader(const std::string& vertex_source, const std::string& fragment_source, const std::string& geometry_source)
{
	GLuint vertex_shader_handle = compile_source(ShaderType::Vertex, vertex_source);
	GLuint fragment_shader_handle = compile_source(ShaderType::Fragment, fragment_source);
	GLuint geometry_shader_handle = compile_source(ShaderType::Geometry, geometry_source);

	link_program(vertex_shader_handle, geometry_shader_handle, fragment_shader_handle);
}

Shader Shader::from_file(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
	std::string vertex_shader_source = read_shader_file(vertex_shader_path);
	std::string fragment_shader_source = read_shader_file(fragment_shader_path);

	return Shader(vertex_shader_source, fragment_shader_source);
}

Shader Shader::from_file(const std::string& vertex_shader_path, const std::string& fragment_shader_path, const std::string& geometry_shader_path)
{
	std::string vertex_shader_source = read_shader_file(vertex_shader_path);
	std::string geometry_shader_source = read_shader_file(geometry_shader_path);
	std::string fragment_shader_source = read_shader_file(fragment_shader_path);

	return Shader(vertex_shader_source, fragment_shader_source, geometry_shader_source);
}

void Shader::use()
{
	glUseProgram(this->program_handle);
}

void Shader::set_uniform(const std::string& name, int value)
{
	use();
	glUniform1i(get_uniform_location(name), value);
}

void Shader::set_uniform(const std::string& name, float value)
{
	use();
	glUniform1f(get_uniform_location(name), value);
}

void Shader::set_uniform(const std::string& name, const glm::vec2& value)
{
	use();
	glUniform2f(get_uniform_location(name), value.x, value.y);
}

void Shader::set_uniform(const std::string& name, const glm::vec3& value)
{
	use();
	glUniform3f(get_uniform_location(name), value.x, value.y, value.z);
}

void Shader::set_uniform(const std::string& name, const glm::vec4& value)
{
	use();
	glUniform4f(get_uniform_location(name), value.x, value.y, value.z, value.w);
}

void Shader::set_uniform(const std::string& name, const glm::mat2& value)
{
	use();
	glUniformMatrix2fv(get_uniform_location(name), 1, false, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name, const glm::mat3& value)
{
	use();
	glUniformMatrix3fv(get_uniform_location(name), 1, false, glm::value_ptr(value));
}

void Shader::set_uniform(const std::string& name, const glm::mat4& value)
{
	use();
	glUniformMatrix4fv(get_uniform_location(name), 1, false, glm::value_ptr(value));
}

GLuint Shader::compile_source(const ShaderType& shader_type, const std::string& source)
{
	GLuint shader = glCreateShader(get_shader_type_code(shader_type));
	
	const char* source_str = source.c_str();
	glShaderSource(shader, 1, &source_str, nullptr);
	glCompileShader(shader);

	if (std::optional<std::string> err = check_shader_error(shader)) {
		std::cerr << err.value() << std::endl;
	}

	return shader;
}

void Shader::link_program(GLuint vertex_shader_handle, std::optional<GLuint> geometry_shader_handle, GLuint fragment_shader_handle)
{
	this->program_handle = glCreateProgram();

	glAttachShader(this->program_handle, vertex_shader_handle);
	if (geometry_shader_handle) {
		glAttachShader(this->program_handle, geometry_shader_handle.value());
	}
	glAttachShader(this->program_handle, fragment_shader_handle);
	glLinkProgram(this->program_handle);

	if (std::optional<std::string> err = check_program_error(this->program_handle)) {
		std::cerr << err.value() << std::endl;
	}
}


constexpr GLuint Shader::get_shader_type_code(const ShaderType& shader_type)
{
	switch (shader_type) {
	case ShaderType::Vertex: return GL_VERTEX_SHADER;
	case ShaderType::Geometry: return GL_GEOMETRY_SHADER;
	case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
	default: return -1;
	}
}

std::optional<std::string> Shader::check_shader_error(GLuint shader_handle)
{
	int success;
	glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);

	if (!success) {
		char info_log[1024];
		glGetShaderInfoLog(shader_handle, 1024, nullptr, info_log);

		std::string log = info_log;
		log.erase(log.find_last_not_of(" \n\r\t") + 1);
		return std::optional<std::string>(log);
	}

	return {};
}

std::optional<std::string> Shader::check_program_error(GLuint program_handle)
{
	int success;
	glGetProgramiv(program_handle, GL_LINK_STATUS, &success);

	if (!success) {
		char info_log[1024];
		glGetProgramInfoLog(program_handle, 1024, nullptr, info_log);

		std::string log = info_log;
		log.erase(log.find_last_not_of(" \n\r\t") + 1);
		return std::optional<std::string>(log);
	}

	return {};
}

GLuint Shader::get_uniform_location(const std::string& name) const
{
	return glGetUniformLocation(get_program_handle(), name.c_str());
}

GLuint Shader::get_program_handle() const
{
	return program_handle;
}

std::string Shader::read_shader_file(const std::string& path)
{
	std::ifstream f(path);
	if (!f.is_open()) {
		throw "Unable to open file " + path;
	}
	std::stringstream buffer;
	buffer << f.rdbuf();

	return buffer.str();
}

