#include "Graphics.h"

#include <iostream>

namespace Graphics
{
	unsigned int current_screen_width= 0;
	unsigned int current_screen_height= 0;
	float current_aspect_ratio= 0;

	SDL_Window *main_window;
	SDL_Renderer *renderer;
	SDL_GLContext opengl_context;
	SDL_Surface *image;
	SDL_Texture *texture;

	unsigned int flags;

	void Initialize()
	{
		SDL_Init(SDL_INIT_VIDEO);

		SDL_Init(SDL_INIT_VIDEO);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		main_window = SDL_CreateWindow("Physics Based Animation", 
									  SDL_WINDOWPOS_CENTERED, 
									  SDL_WINDOWPOS_CENTERED, 
									  DEFAULT_SCREEN_WIDTH, 
									  DEFAULT_SCREEN_HEIGHT, 
									  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		current_screen_width= DEFAULT_SCREEN_WIDTH;
		current_screen_height= DEFAULT_SCREEN_HEIGHT;

		renderer = SDL_CreateRenderer(main_window, -1, 0);
		opengl_context = SDL_GL_CreateContext(main_window);

		SDL_GL_SetSwapInterval(0);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glPointSize(4);

		glOrtho(-40, 40, -20, 20, 0, 100);
	}

	void Conclude()
	{
		SDL_Quit();
	}

	void ChangeResolution(unsigned int screen_width, unsigned int screen_height)
	{
		current_screen_width= screen_width;
		current_screen_height= screen_height;
		current_aspect_ratio= current_screen_width/ (float)current_screen_height;

		SDL_SetWindowSize(main_window, screen_width, screen_height);
	}

	void SaveScreenToFile()
	{
		//SDL_SaveBMP(screen, "screenshot.bmp");
	}

	float GetCurrentAspectRatio()
	{
		return current_screen_height/ (float)current_screen_width;
	}

	void Render(vector<Particle *> particles)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBegin(GL_POINTS);
		for(unsigned int i= 0; i< particles.size(); i++)
			glVertex3f(particles[i]->position[0], particles[i]->position[1], 0.0f);
		glEnd();

		SDL_GL_SwapWindow(main_window);
	}
}