#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#define GLEW_STATIC

enum days {MON, TUE, WED, THUR, FRI, SAT};


int main(int argc, char** argv) {
	int sdl_success;
	sdl_success = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	SDL_Window *win;
	win = SDL_CreateWindow("Open GL Testing",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			1024,768, SDL_WINDOW_OPENGL);

	if (sdl_success == 0) {
		printf("SDL2 Succesfully Initialized\n");
	} else {
		printf("SDL2 Failed to initialize: %s \n", SDL_GetError());
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GLContext glcontext = SDL_GL_CreateContext(win);	

	printf("GL Version used: (%s)\n", glGetString(GL_VERSION));
	printf("GL Vendor: (%s)\n", glGetString(GL_VENDOR));	

	// What does glewInit() actually do?
	GLenum err = glewInit();

	if (err == GLEW_OK){
		printf("GLEW Succesfully Initialized\n");
	}

	glViewport(0,0, 1024, 768);
	
	float vertices_a[] = {
		-0.5f, 0.0f, 0.0f, // Triangle one
		0.0f, 0.0f, 0.0f,
		-0.25f, 0.5f, 0.0f,
	}, vertices_b[] = {
		0.0f, 0.0f, 0.0f,// Triangle two
		0.5f, 0.0f, 0.0f,
		0.25f, 0.5f, 0.0f,
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3	
	};


	unsigned int vbo_list[2];
	unsigned int vao_list[2];
	unsigned int ebo;

	glGenBuffers(2, vbo_list);
	glGenVertexArrays(2, vao_list);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao_list[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_list[0]);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_a), vertices_a, GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// aside
	int x[5] = {1,2,3,4,5};
	int *p = (void*)&x+4;
	printf("Address: %p\n", &x[1]);
	printf("Address: %d\n", *p);
	//

	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
		"}\0";

	const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
        	"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";

	const char *fragmentShaderSource_yellow = "#version 330 core\n"
	"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
        	"FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
		"}\0";

	unsigned int vertexShader, fragmentShader, fragmentShader_yellow;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShader_yellow = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glShaderSource(fragmentShader_yellow, 1, &fragmentShaderSource_yellow, NULL);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	glCompileShader(fragmentShader_yellow);
	

	// Check if compilation succeeded
	int success;
	char infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s\n", infoLog);
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s\n", infoLog);
	}

	glGetShaderiv(fragmentShader_yellow, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragmentShader_yellow, 512, NULL, infoLog);
		printf("ERROR:SHADER::FRAGMENT::COMPILATION_FAILED: %s\n", infoLog);
	}
	
	// Create shader program ( link vertex and fragment shader here )
	unsigned int shaderProgram, shaderProgram_yellow;
	shaderProgram = glCreateProgram();
	shaderProgram_yellow = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glAttachShader(shaderProgram_yellow, vertexShader);
	glAttachShader(shaderProgram_yellow, fragmentShader_yellow);
	glLinkProgram(shaderProgram);
	glLinkProgram(shaderProgram_yellow);

	// Check if linking succeeded
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}
	glGetProgramiv(shaderProgram_yellow, GL_LINK_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(shaderProgram_yellow, 512, NULL, infoLog);
	}

	// We no longer need the shaders, as we now have the program born from them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader_yellow);

	// We've sent the vertex data to the GPU as well as told it how to process it through shaders,
	// but OGL doesn't know how to interpret the vertex data in memory, and it doesn't
	// yet know how how to connect the data to the vertex shader's attributes

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao_list[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_list[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_b), vertices_b, GL_STATIC_DRAW);	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);	


	glUseProgram(shaderProgram);

	glClearColor(0.2f,0.3f,0.4f,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(vao_list[0]);
	glUseProgram(shaderProgram_yellow);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	SDL_GL_SwapWindow(win);
	SDL_Delay(1000);
	
	printf("\n");
	
}
