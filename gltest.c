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

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,2);
	SDL_GLContext glcontext = SDL_GL_CreateContext(win);	
	
	// Why do we need this?
	GLenum err = glewInit();
	if (err == GLEW_OK){
		printf("Success");
	}
	glClearColor(0,0,100,1);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(win);
	SDL_Delay(1000);

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f

	};
	
	printf("GL Version: (%s)",glGetString(GL_VERSION));

	unsigned int vbo;

	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	
}
