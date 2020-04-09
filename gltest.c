#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#define GLEW_STATIC

int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	SDL_Window *win;
	win = SDL_CreateWindow("Open GL Testing",
		       	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			1024,768, SDL_WINDOW_OPENGL);
	printf("%d", SDL_WINDOWPOS_UNDEFINED);

	SDL_GLContext glcontext = SDL_GL_CreateContext(win);
	
	// Why do we need this?
	glewInit();

	glClearColor(0,0,100,1);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(win);
	SDL_Delay(1000);

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f

	};

}
