// OpenGL_Test.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <string>
#include <glad.h>
#include <SDL.h>
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
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5,  0.0f
	};

	// Vertex shader (yeah I know this is cursed, it's temporary)
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main() {\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\n";

	// Fragment shader
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2, 1.0f);\n"
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

	SDL_GLContext context = SDL_GL_CreateContext(win); // Make the context
	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress); //???
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // Only CORE PROFILE allowed in here
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // OpenGL 3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); // OpenGL 3.3
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // Set OpenGL resolution/viewport thingy

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
		logSDLError(cout, infoLog);
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
		logSDLError(cout, infoLog);
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
		logSDLError(cout, infoLog);
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// Delete shader objects now that the program has been set up
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
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
		glClear(GL_COLOR_BUFFER_BIT); // I honestly have no idea what this does

		// Draw triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		SDL_GL_SwapWindow(win); // VSYNC?
	}

	// Clean up
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}

// Error message logger/printer
void logSDLError(ostream& os, const string& msg) {
	os << msg << " error: " << SDL_GetError() << endl;
}