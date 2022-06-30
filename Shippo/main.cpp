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
#include "LightCube.hpp"
#include "Texture.hpp"
#include "Model.hpp"
#include "Water.hpp"
#include "Ship.hpp"
#include <chrono>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Rozmiary ekranu
int screen_width = 1280;
int screen_height = 720;

const float sensitivity = 0.4f;
float camera_distance = 8.0f;

const glm::vec3 WHITE_LIGHT_COLOR = glm::vec3(1.0, 1.0, 1.0);
const glm::vec3 GREEN_LIGHT_COLOR = glm::vec3(0.3, 1.0, 0.3);
const glm::vec3 RED_LIGHT_COLOR = glm::vec3(1.0, 0.3, 0.3);
const glm::vec3 YELLOW_LIGHT_COLOR = glm::vec3(1.0, 1.0, 0.3);	

	
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

	// Create ship object
	Ship ship;

	// Defining camera
	float fov = 45.f;
	float pitch = 0;
	float yaw = 0;

	// Setup water plane
	auto start_time = std::chrono::high_resolution_clock::now();
	Water water_plane(1200);
	auto current_time = std::chrono::high_resolution_clock::now();
	std::cout << "Water generation took: " << std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() << " ms" << std::endl;

	// Water waves constants
	int static_waves = 0;
	float wave_amplitude = 0.3;
	float wave_length = 6.4;

	// Light constants
	LightCube light = LightCube(glm::vec3(5.2f, 7.0f, 2.0f), WHITE_LIGHT_COLOR);
	LightCube second_light = LightCube(glm::vec3(14.0f, 11.0f, 7.0f), WHITE_LIGHT_COLOR);

	glm::vec3 sun_color = glm::vec3(1.0, 1.0, 1.0);
	glm::vec3 sun_direction = glm::normalize(glm::vec3(0.64, -0.56, -0.5f));
	
	bool running = true;
	uint64_t last_time = SDL_GetPerformanceCounter();
	while (running) {
		// Calculate delta time
		uint64_t now = SDL_GetPerformanceCounter();
		double delta_time = (double)((now - last_time) * 1000 / (double)SDL_GetPerformanceFrequency()) * 0.001;
		last_time = now;
		//std::cout << delta_time << "\t FPS: " << 1 / delta_time << std::endl;
		//std::cout << "Amplitude: " << wave_amplitude << ", length: " << wave_length << std::endl;
		
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

				// Ship
				case SDLK_w: {
					ship.move({ 1, 0, 0 });
				} break;
				case SDLK_s: {
					ship.move({ -1, 0, 0 });
				} break;
				case SDLK_a: {
					ship.move({ 0, 0, -1 });
				} break;
				case SDLK_d: {
					ship.move({ 0, 0, 1 });
				} break;

				// Light 1
				case SDLK_y: {
					light.set_position(light.get_position() + glm::vec3(1, 0, 0));
				} break;
				case SDLK_h: {
					light.set_position(light.get_position() + glm::vec3(-1, 0, 0));
				} break;
				case SDLK_g: {
					light.set_position(light.get_position() + glm::vec3(0, 0, -1));
				} break;
				case SDLK_j: {
					light.set_position(light.get_position() + glm::vec3(0, 0, 1));
				} break;

				// Light 1 Colors
				case SDLK_1: {
					light.set_color(WHITE_LIGHT_COLOR);
				} break;
				case SDLK_2: {
					light.set_color(RED_LIGHT_COLOR);
				} break;
				case SDLK_3: {
					light.set_color(GREEN_LIGHT_COLOR);
				} break;
				case SDLK_4: {
					light.set_color(YELLOW_LIGHT_COLOR);
				} break;

				// Light 2 Colors
				case SDLK_6: {
					second_light.set_color(WHITE_LIGHT_COLOR);
				} break;
				case SDLK_7: {
					second_light.set_color(RED_LIGHT_COLOR);
				} break;
				case SDLK_8: {
					second_light.set_color(GREEN_LIGHT_COLOR);
				} break;
				case SDLK_9: {
					second_light.set_color(YELLOW_LIGHT_COLOR);
				} break;

				// Waves
				case SDLK_z: {
					static_waves = 0;
				} break;
				case SDLK_x: {
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

				// Debug
				case SDLK_l: {
					ship.print_textures();
				} break;
				case SDLK_k: {
					std::cout << "Amplitude: " << wave_amplitude << ", length: " << wave_length << std::endl;
				} break;
				default: break;
				}
			} break;
			case SDL_KEYUP: {
				switch (event.key.keysym.sym) {
				case SDLK_w:
				case SDLK_s:
				case SDLK_a:
				case SDLK_d: {
					ship.move({ 0, 0, 0 });
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
				camera_distance -= (float)event.wheel.y;
				if (camera_distance < 4.0f)
					camera_distance = 4.0f;
				if (camera_distance > 30.f)
					camera_distance = 30.0f;
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
		if (pitch < 4) {
			pitch = 4;
		}

		// Calculate camera position based on view direction
		glm::vec3 direction{};
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction = glm::normalize(direction) * camera_distance;
		glm::vec3 camera_pos = ship.get_position() + direction;

		// Model-view-projection matrix calculations
		glm::mat4 view = glm::lookAt(camera_pos, ship.get_position(), glm::vec3(0.0, 1.0, 0.0)); //position, target, up vector
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)screen_width / (float)screen_height, 0.1f, 1200.0f);

		// Set clear color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Clear window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw with OpenGL
		// Ship
		ship.get_shader().use();
		ship.get_shader().set_uniform("lightColor", light.get_color());
		ship.get_shader().set_uniform("lightPos", light.get_position());
		ship.get_shader().set_uniform("viewPos", camera_pos);

		ship.update(delta_time, view, projection);
		ship.render(ship.get_shader());

		// Water
		water_plane.update(delta_time, view, projection);
		auto& water_shader = water_plane.get_shader();
		// Wave uniforms
		water_shader.set_uniform("waveAmplitude", wave_amplitude);
		water_shader.set_uniform("waveLength", wave_length);
		water_shader.set_uniform("waveStatic", static_waves);

		// Light uniforms
		water_shader.set_uniform("lightColor", light.get_color());
		water_shader.set_uniform("lightPos", light.get_position());
		water_shader.set_uniform("sunColor", sun_color);
		water_shader.set_uniform("sunDirection", sun_direction);
		water_shader.set_uniform("viewPos", camera_pos);
		water_plane.render(water_shader);

		// Light cubes
		light.update(view, projection);
		light.render(light.get_shader());

		second_light.update(view, projection);
		second_light.render(second_light.get_shader());

		// Update window with OpenGL render results
		SDL_GL_SwapWindow(window);
		SDL_Delay(10);
	}


	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
};

