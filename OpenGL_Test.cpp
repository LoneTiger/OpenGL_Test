// OpenGL_Test.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <string>
#include <SDL.h>
#undef main

using namespace std;

int main()
{
	cout << "Hello bruh" << endl;

	//Initialize SDL2
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "Error init SDL2_Video: " << SDL_GetError() << endl;
		return 1;
	}

	//Create window
	SDL_Window* win = SDL_CreateWindow("OpenGL_Test", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		cout << "Error creating window: " << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}

	//Create renderer
	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) {
		SDL_DestroyWindow(win);
		cout << "Error creating SDL2_Render: " << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}

	//Load image
	char* basePath = SDL_GetBasePath();
	string baseDir = basePath;
	SDL_free(basePath);
	string imagePath = baseDir + "Trollface.bmp";
	SDL_Surface* bmp = SDL_LoadBMP(imagePath.c_str());
	if (bmp == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		cout << "Error loading bitmap: " << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}

	//Create texture to display the image
	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, bmp);
	SDL_FreeSurface(bmp);
	if (tex == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		cout << "Error creating texture: " << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}

	//Draw the texture
	SDL_RenderClear(ren);
	SDL_RenderCopy(ren, tex, NULL, NULL);
	SDL_RenderPresent(ren);
	SDL_Delay(5000);

	//Clean up
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}
