// OpenGL_Test.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <string>
#include <SDL.h>
#undef main

using namespace std;

void logSDLError(ostream& os, const string& msg);
SDL_Texture* loadTexture(const string& file, SDL_Renderer* ren);

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
	SDL_Window* win = SDL_CreateWindow("OpenGL_Test", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
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

	// Load image
	/*char* basePath = SDL_GetBasePath();
	string baseDir = basePath;
	SDL_free(basePath);
	string imagePath = baseDir + "Trollface.bmp";
	SDL_Surface* bmp = SDL_LoadBMP(imagePath.c_str());
	if (bmp == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		logSDLError(cout, "Bitmap load");
		SDL_Quit();
		return 1;
	}
	*/

	// Load image to texture
	SDL_Texture* tex = loadTexture("Trollface.bmp", ren);

	// Create texture to display the image
	/*
	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, bmp);
	SDL_FreeSurface(bmp);
	if (tex == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		logSDLError(cout, "Texture creation");
		SDL_Quit();
		return 1;
	}
	*/

	// Draw the texture
	SDL_RenderClear(ren);
	SDL_RenderCopy(ren, tex, NULL, NULL);
	SDL_RenderPresent(ren);
	SDL_Delay(5000);

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