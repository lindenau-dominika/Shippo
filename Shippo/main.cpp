#include<iostream>
#include<stdio.h>
#include<math.h>
#include<vector>
#include<SDL.h>
#include<glad/glad.h>

//rozmiary ekranu
const int Screen_Width = 1280;
const int Screen_Height = 920;

int main(int argc, char* args[])
{
 //Okno w którym renderowane bêd¹ obiekty
	SDL_Window* window = NULL;

	SDL_Surface* screenSurface = NULL;

	//Inicjalizacja SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
	}
	else
	{

		//Tworzenie okna
		window = SDL_CreateWindow("Shippo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Screen_Width, Screen_Height, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			std::cout << "Window could not be created! SDL_Error: " << SDL_GetError();
		}
		else
		{	
			while (true) {
				//Get window surface
				screenSurface = SDL_GetWindowSurface(window);

				//fill the surface black
				SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));

				//Updating the surface
				SDL_UpdateWindowSurface(window);

				//Tymczasowa funkcja wait aby okno nie znika³o od razu
			}
		}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
};

/*
SDL2
Po SDLu jeszcze to dograj: GLAD
https://learnopengl.com/Getting-started/Creating-a-window#:~:text=configuration%20of%20GLFW.-,GLAD,-We%27re%20still%20not
Zrób repo jeszcze 
*/