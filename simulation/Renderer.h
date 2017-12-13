/*
	Klasa renderujaca
*/

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <stdio.h>

#include <SDL.h>

#include "SimulationState.h"
#include "TileMap.h"

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
#define DEFAULT_FULLSCREEN false

class Renderer
{
	public:
		Renderer();
		~Renderer();
		
		void init();
		void render( SimulationState *state );
		
		int getWidth() { return this->width; }
		int getHeight() { return this->height; }
	
	private:
		void loadResources();
		
		int width, height;	// wymiary ekranu
		bool fullscreen;
		
		SDL_Window *window = NULL;	// okno aplikacji
		SDL_Surface *screen = NULL;	// zawartosc okna
		
		// zasoby graficzne - tekstury
		struct Resources
		{
			static const int NUM_CARS = 10;
			static const int NUM_DIRECTIONS = 4;
			static const int NUM_COLORS = 3;
			
			SDL_Surface *textures[ TileMap::TILE_TYPES::NUM_TILE_TYPES ];
			SDL_Surface *carTextures[ NUM_CARS ][ NUM_DIRECTIONS ];
			SDL_Surface *lightTextures[ NUM_COLORS ][ NUM_DIRECTIONS ];
		} resources;
};

#endif // _RENDERER_H