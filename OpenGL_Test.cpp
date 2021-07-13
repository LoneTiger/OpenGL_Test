// OpenGL_Test.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <string>
#include <glad.h>
#include <SDL.h>
#include "shader.h"
#undef main

using namespace std;

void logSDLError(ostream& os, const string& msg);

int main()
{
	// Vars
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	// Triangle verticies
	float verticies[] = {
		// Coordinates			// Colors
		-0.5f,  0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		-0.25f,  0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
		 0.0f, -0.0f, 0.0f,		0.0f, 0.0f, 1.0f,

		 0.0f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
		 0.25f, 0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f
	};

	// Rectangle verticies
	float verticies2[] = {
		// Coordinates			// Colors
		-0.75f, -0.75f, 0.0f,	0.0f, 0.0f, 1.0f,
		-0.75f, -0.25f, 0.0f,	0.0f, 1.0f, 0.0f,
		-0.25f, -0.25f, 0.0f,	1.0f, 0.0f, 0.0f,
		-0.25f, -0.75f, 0.0f,	0.0f, 1.0f, 0.0f
	};

	// Indices
	unsigned int indices[] = {
		0, 1, 2, 3, 4, 5
	};

	// Rectangle Indices
	unsigned int indices2[] = {
		0, 1, 2, 2, 3, 0
	};

	cout << "Hello bruh" << endl;

	// Initialize SDL2
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logSDLError(cout, "Video_init");
		SDL_Quit();
		return 1;
	}

	// Create window
	SDL_Window* win = SDL_CreateWindow("OpenGL_Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		logSDLError(cout, "Creating window");
		SDL_Quit();
		return 1;
	}

	// Time for OpenGL!! :D

	// Set everything up
	SDL_GLContext context = SDL_GL_CreateContext(win); // Make the context
	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress); // GLAD
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // Only CORE PROFILE allowed in here
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // OpenGL 3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); // OpenGL 3.3
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // Set OpenGL resolution/viewport thingy

	// Print max number of vertex attributes (I'm curious)
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	cout << "Max number of vertex attributes: " << nrAttributes << endl;

	// Create shader/program object
	Shader ourShader("../shader.vsh", "../shader.fsh");

	// VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Vertex buffer object
	unsigned int VBO; // Variable for buffer ID
	glGenBuffers(1, &VBO); // Create the buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the buffer as ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW); // Load the vertex array to the buffer

	// Vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Color attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Now all the same stuff for the rectangle

	// VAO
	unsigned int VAO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	// Vertex buffer object
	unsigned int VBO2;
	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies2), verticies2, GL_STATIC_DRAW);

	// Vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Color attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// EBO
	unsigned int EBO2;
	glGenBuffers(1, &EBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);


	ourShader.use();


	// Temp wireframe rendering
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// Vars for main loop
	SDL_Event e;
	bool quit = false;
	// Main loop
	while (!quit) {
		// Check if it's time to quit
		while (SDL_PollEvent(&e)) {
			// Window closed?
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			// Key pressed?
			//if (e.type == SDL_KEYDOWN) {
			//	quit = true;
			//}
			// Mouse clicked?
			//if (e.type == SDL_MOUSEBUTTONDOWN) {
			//	quit = true;
			//}
		}
		// OpenGL :D

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Red, Green, Blue, Alpha
		glClear(GL_COLOR_BUFFER_BIT); // Clear the screen with above color

		// Draw triangle
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(VAO2);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		SDL_GL_SwapWindow(win); // VSYNC?
	}

	// Clean up
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}

// SDL Error message logger/printer
void logSDLError(ostream& os, const string& msg) {
	os << msg << " error: " << SDL_GetError() << endl;
}