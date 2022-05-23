#include <iostream>
#include <vector>
#include <cmath>
#include <SDL.h>
#include <glm/glm.hpp>
#include <glad/glad.h>

//rozmiary ekranu
const int screen_width = 1280;
const int screen_height = 720;

int main(int argc, char* args[])
{
 	// Okno w którym renderowane bêd¹ obiekty
	SDL_Window* window = NULL;
	SDL_Surface* screen_surface = NULL;

	// Inicjalizacja SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	// Set the GL version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

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
		std::cout << "Couldn't create OpenGL context! SDL_Error: " << SDL_GetError() << std::endl;
	}
	
	while (true) {
		// Get window surface
		screen_surface = SDL_GetWindowSurface(window);

		// Fill the surface black
		SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, 0, 0, 0));

		// Updating the surface
		SDL_UpdateWindowSurface(window);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
};