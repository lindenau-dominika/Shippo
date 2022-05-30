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
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Rozmiary ekranu
int screen_width = 1280;
int screen_height = 720;

const float sensitivity = 0.1f;
float camera_distance = 5.0f;


	
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

	// Import the model file
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile("resources/models/stateczek1.obj", aiProcess_Triangulate | aiProcess_FlipUVs);

	// Reporting failure of importing the file
	if (scene == nullptr)
	{
		std::cout << importer.GetErrorString() << std::endl;
		return -1;
	}
	

	std::vector<float> vertices;
	std::vector<float> tex_coords;
	std::vector<unsigned int> indices;

	std::cout << "Num of meshes: " << scene->mNumMeshes << std::endl;
	for (int i = 0; i < scene->mNumMeshes; i++) {
		auto mesh = scene->mMeshes[i];
		std::cout << "Mesh: " << i << std::endl;
		std::cout << mesh->mNumVertices << std::endl;
		
		for (int j = 0; j < mesh->mNumFaces; j++) {
			auto &face = mesh->mFaces[j];
			for (int k = 0; k < face.mNumIndices; k++) {
				indices.push_back(face.mIndices[k]);
			}
		}

		for (int j = 0; j < mesh->mNumVertices; j++) {
			auto vertex = mesh->mVertices[j];
			std::cout << "Vertex: " << j << std::endl;
			vertices.push_back(vertex.x);
			vertices.push_back(vertex.y);
			vertices.push_back(vertex.z);

			if (mesh->mTextureCoords[0] != nullptr) {
				auto tex_coord = mesh->mTextureCoords[0][j];
				tex_coords.push_back(tex_coord.x);
				tex_coords.push_back(tex_coord.y);
			}
		}
		std::cout << mesh->mNumFaces << std::endl;
	}

	// Defining camera
	glm::vec3 camera_pos = glm::vec3(4.f, 6.f, -5.0f);
	glm::vec3 camera_target = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 camera_front = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 camera_up = glm::vec3(0.f, 0.f, -1.f);
	
	float fov = 45.f;
	float pitch = 0;
	float yaw = 0;

	Texture texture = Texture();
	texture.from_file("resources/textures/graph_paper.jpg");

	unsigned int ebo;
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	bool running = true;
	while (running) {
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
		//model = glm::rotate(model, glm::radians(SDL_GetTicks() / 10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 view = glm::lookAt(camera_pos + camera_target, camera_target, up); //position, target, up vector
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)screen_width / (float)screen_height, 0.1f, 100.0f);
		glm::mat4 mvp = projection * view * model;

		// Set clear color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Clear window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw with OpenGL
		shader.use();
		shader.set_uniform("mvp", mvp);

		texture.bind();

		glEnableVertexAttribArray(shader.get_attribute_location("position"));
		glVertexAttribPointer(shader.get_attribute_location("position"), 3, GL_FLOAT, false, 0, (void *)vertices.data());

		glEnableVertexAttribArray(shader.get_attribute_location("vTexCoord"));
		glVertexAttribPointer(shader.get_attribute_location("vTexCoord"), 2, GL_FLOAT, false, 0, (void*)tex_coords.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
		
		glDisableVertexAttribArray(shader.get_attribute_location("position"));
		glDisableVertexAttribArray(shader.get_attribute_location("vTexCoord"));


		// Update window with OpenGL render results
		SDL_GL_SwapWindow(window);
	}


	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
};

