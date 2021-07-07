// OpenGL_Test.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <string>
#include <SDL.h>
#undef main

using namespace std;

void logSDLError(ostream& os, const string& msg);
SDL_Texture* loadTexture(const string& file, SDL_Renderer* ren);
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y);

int main()
{
	// Vars
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	cout << "Hello bruh" << endl;

	// Initialize SDL2
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logSDLError(cout, "Video_init");
		return 1;
	}

	// Create window
	SDL_Window* win = SDL_CreateWindow("OpenGL_Test", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		logSDLError(cout, "Creating window");
		SDL_Quit();
		return 1;
	}

	// Create renderer
	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) {
		SDL_DestroyWindow(win);
		logSDLError(cout, "SDL_Render");
		SDL_Quit();
		return 1;
	}

	// Load image to texture
	SDL_Texture* tex = loadTexture("Trollface.bmp", ren);
	
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
		// Render
		SDL_RenderClear(ren);
		renderTexture(tex, ren, 0, 0);
		SDL_RenderPresent(ren);
	}

	// Clean up
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}

// Error message logger/printer
void logSDLError(ostream& os, const string& msg) {
	os << msg << " error: " << SDL_GetError() << endl;
}

// Texture loading function
// Takes path to image and renderer, returns pointer to texture
SDL_Texture* loadTexture(const string& file, SDL_Renderer* ren) {
	// Init as nullptr
	SDL_Texture* texture = nullptr;
	// Load image
	SDL_Surface* loadedImage = SDL_LoadBMP(file.c_str());
	// Make sure we didn't fail to load the image
	if (loadedImage != nullptr) {
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		SDL_FreeSurface(loadedImage);
		//Make sure the texture was created
		if (texture == nullptr) {
			logSDLError(cout, "CreateTextureFromSurface");
		}
	}
	else {
		logSDLError(cout, "LoadBMP");
	}
	return texture;
}

// Texture renderer
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y) {
	// Set destination rectangle position
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	// Get width and height from texture
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);
}