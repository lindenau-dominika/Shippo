#include <iostream>
#include <vector>
#include <cmath>
#include <SDL.h>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Shader.hpp"
#include "Texture.hpp"
#include "Model.hpp"
#include "Water.hpp"
#include <chrono>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Rozmiary ekranu
int screen_width = 1280;
int screen_height = 720;

const float sensitivity = 0.1f;
float camera_distance = 8.0f;


	
int main(int argc, char* args[])
{
	// Okno w którym renderowane bêd¹ obiekty
	SDL_Window* window = NULL;

	// Inicjalizacja SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL failed to initalize! SDL_Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	// Tworzenie okna
	window = SDL_CreateWindow("Shippo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (window == NULL)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	// Create the OpenGL context
	SDL_GLContext gl = SDL_GL_CreateContext(window);
	if (gl == nullptr) {
		std::cout << "Failed create OpenGL context! SDL_Error: " << SDL_GetError() << std::endl;
	}

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}

	// Mouse capture
	if (SDL_GetRelativeMouseMode() == SDL_FALSE) 
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}

	
	
	// Tell OpenGL about our window size
	glViewport(0, 0, screen_width, screen_height);
	glEnable(GL_DEPTH_TEST);

	Shader shader = Shader::from_file("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");


	// Import the model
	Model object_model("resources/models/Humvee.obj");
	glm::vec3 position = glm::vec3(0.0);

	// Defining camera
	glm::vec3 camera_pos = glm::vec3(4.f, 6.f, -5.0f);
	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 camera_front = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 camera_up = glm::vec3(0.f, 0.f, -1.f);
	
	float fov = 45.f;
	float pitch = 0;
	float yaw = 0;

	Texture albedo_texture = Texture("resources/textures/Humvee_Albedo.png", TextureType::Albedo);
	Texture normal_texture = Texture("resources/textures/Humvee_Normal.png", TextureType::Normal);
	Texture metallic_texture = Texture("resources/textures/Humvee_Metallic.png", TextureType::Metallic);

	// Setup water plane
	auto start_time = std::chrono::high_resolution_clock::now();
	Water water_plane(1200);
	auto current_time = std::chrono::high_resolution_clock::now();
	std::cout << "Water generation took: " << std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() << " ms" << std::endl;


	int static_waves = 0;
	float wave_amplitude = 0.3;
	float wave_length = 6.4;

	bool running = true;
	uint64_t last_time = SDL_GetPerformanceCounter();
	while (running) {
		// Calculate delta time
		uint64_t now = SDL_GetPerformanceCounter();
		double delta_time = (double)((now - last_time) * 1000 / (double)SDL_GetPerformanceFrequency()) * 0.001;
		last_time = now;
		//std::cout << delta_time << "\t FPS: " << 1 / delta_time << std::endl;
		std::cout << "Amplitude: " << wave_amplitude << ", length: " << wave_length << std::endl;

		// Poll all window events
		SDL_Event event;
		float xoffset = 0, yoffset = 0;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN: {
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE: {
					running = false;
				} break;
				case SDLK_w: {
					position.z += 100.0f * delta_time;
				} break;
				case SDLK_s: {
					position.z -= 100.0f * delta_time;
				} break;
				case SDLK_a: {
					position.x += 100.0f * delta_time;
				} break;
				case SDLK_d: {
					position.x -= 100.0f * delta_time;
				} break;
				case SDLK_f: {
					static_waves = 0;
				} break;
				case SDLK_g: {
					static_waves = 1;
				} break;
				case SDLK_UP: {
					wave_length += 0.2;
				} break;
				case SDLK_DOWN: {
					wave_length -= 0.2;
				} break;
				case SDLK_LEFT: {
					wave_amplitude -= 0.1;
				} break;
				case SDLK_RIGHT: {
					wave_amplitude += 0.1;
				} break;
				default: break;
				}
			} break;
			case SDL_WINDOWEVENT: {
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					// Tell OpenGL that we resized the window
					glViewport(0, 0, event.window.data1, event.window.data2);
				}
			} break;
			case SDL_QUIT: {
				running = false;
			} break;
			case SDL_MOUSEMOTION: {
				xoffset = event.motion.xrel;
				yoffset = event.motion.yrel;
			} break;
			case SDL_MOUSEWHEEL: {
				fov -= (float)event.wheel.y;
				if (fov < 10.0f) 
					fov = 10.0f;
				if (fov > 60.f) 
					fov = 60.f;
			}
			default: break;
			}
		}

		// Calculate new yaw and pitch
		yaw += xoffset * sensitivity;
		pitch += yoffset * sensitivity;

		// Clamp pitch
		if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		if (pitch < -89.0f) {
			pitch = -89.0f;
		}

		// Calculate camera position based on view direction
		glm::vec3 direction{};
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		camera_pos = glm::normalize(direction) * camera_distance;

		// Model-view-projection matrix calculations
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		//model = glm::rotate(model, glm::radians(SDL_GetTicks() / 10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 view = glm::lookAt(camera_pos + position, position, up); //position, target, up vector
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)screen_width / (float)screen_height, 0.1f, 1200.0f);
		glm::mat4 mvp = projection * view * model;

		// Set clear color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Clear window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw with OpenGL
		shader.use();
		shader.set_uniform("mvp", mvp);
		shader.set_uniform("model", model);
		shader.set_uniform("lightColor", glm::vec3(1.0, 1.0, 1.0));
		shader.set_uniform("lightPos", glm::vec3(5.2f, 10.0f, 2.0f));
		shader.set_uniform("viewPos", camera_pos + position);
		shader.set_uniform("albedoTexture", albedo_texture.unit_index());
		shader.set_uniform("normalTexture", normal_texture.unit_index());
		shader.set_uniform("metallicTexture", metallic_texture.unit_index());

		albedo_texture.bind();
		normal_texture.bind();
		metallic_texture.bind();
		object_model.render(shader);

		water_plane.update(delta_time, view, projection);
		auto& water_shader = water_plane.get_shader();
		water_shader.set_uniform("waveAmplitude", wave_amplitude);
		water_shader.set_uniform("waveLength", wave_length);
		water_shader.set_uniform("waveStatic", static_waves);
		water_shader.set_uniform("lightColor", glm::vec3(1.0, 1.0, 1.0));
		water_shader.set_uniform("lightPos", glm::vec3(5.2f, 10.0f, 2.0f));
		water_shader.set_uniform("viewPos", camera_pos + position);
		water_plane.render(water_shader);

		// Update window with OpenGL render results
		SDL_GL_SwapWindow(window);
	}


	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
};

