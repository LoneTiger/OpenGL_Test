// OpenGL_Test.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <string>
#include <glad.h>
#include <SDL.h>
#undef main

using namespace std;

void logSDLError(ostream& os, const string& msg);
void logShaderError(ostream& os, const string& msg, int type);

int main()
{
	// Vars
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	// Triangle verticies
	float verticies[] = {
		// Coordinates		// Colors
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	// Indices
	unsigned int indices[] = {
		0, 1, 2
	};

	// Vertex shader (yeah I know this is cursed, it's temporary)
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec3 ourColor;\n"
		"void main() {\n"
		"	gl_Position = vec4(aPos, 1.0);\n"
		"	ourColor = aColor;"
		"}\n";

	// Fragment shader
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor;\n"
		"void main() {\n"
		"	FragColor = vec4(ourColor, 1.0);\n"
		"}\n";


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

	// Create vertex shader object
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Attach the vertex shader source and compile it
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Make sure the compilation is successfull
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	// If it failed
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		logShaderError(cout, infoLog, 0);
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}
	
	// Create fragment shader object
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check for compilation errors again
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		logShaderError(cout, infoLog, 1);
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	// Shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check for program errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		logShaderError(cout, infoLog, 2);
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	// Delete shader objects now that the program has been set up
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

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
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
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

// OpenGL shader error message logger
void logShaderError(ostream& os, const string& msg, int type) {

	/*
	* Types:
	* 0 - vertex shader
	* 1 - fragment shader
	* 2 - program
	*/

	switch (type) {
		case 0:
			os << "Vertex shader error: " << msg << endl;
			break;
		case 1:
			os << "Fragment shader error: " << msg << endl;
			break;
		case 2:
			os << "Shader program error: " << msg << endl;
	}
}