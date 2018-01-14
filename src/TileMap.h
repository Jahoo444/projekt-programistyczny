/*
	Mapa ze skrzyzowaniami, skladajaca sie z plytek
	Tu w formie tablicy dwuwymiarowej zdefiniowane sa drogi i skrzyzowania:
	ich kierunki, typy, stany swiatel itp.
*/

#ifndef _TILE_MAP_H_
#define _TILE_MAP_H_

#include <vector>
#include <tuple>
#include <map>

#include <SDL.h>

#include "Light.h"

class TileMap
{
	public:
		static const int TILE_SIZE = 64;	// rozmiar plytki

		static const int GREEN_LIGHT_TIME = 8000;
		static const int RED_LIGHT_TIME = 8000;

		static const int SWITCH_TIME = 8000;

		// typy plytek
		enum TILE_TYPES
		{
			GRASS,
			ROAD_UP,
			ROAD_DOWN,
			ROAD_LEFT,
			ROAD_RIGHT,
			CROSSROADS_UL,
			CROSSROADS_LL,
			CROSSROADS_UR,
			CROSSROADS_LR,

			NUM_TILE_TYPES
		};

		TileMap();

		void load( const char *path );	// wczytywanie mapy z pliku
		void update();

		void takeTile( int col, int row ) { if( col >= 0 && col < this->width && row >= 0 && row < this->height ) this->taken[ col ][ row ]++; }	// zajecie plytki przez auto
		void leaveTile( int col, int row ) { if( col >= 0 && col < this->width && row >= 0 && row < this->height ) this->taken[ col ][ row ]--; }	// opuszczenie plytki przez auto

		int getWidth() { return this->width; }
		int getHeight() { return this->height; }

		char **getData() { return this->data; }

		std::vector< std::pair< int, int > > *getSpawners() { return &( this->spawners ); }
		std::map< std::pair< int, int >, Light* > *getLights() { return &( this->lights ); }

		bool tileTaken( int col, int row ) { if( col >= 0 && col < this->width && row >= 0 && row < this->height ) return this->taken[ col ][ row ]; else return false; }	// czy plytka zajeta
		Light::COLORS getColor( int col, int row );
		Light::COLORS getColor( Light::DIRECTIONS dir );
		Light::DIRECTIONS getLightDirection( int col, int row );
		bool onCrossroads( int col, int row )
		{
			if( col < 0 || col >= this->width || row < 0 || row >= this->height )
				return false;
			return this->data[ col ][ row ] >= CROSSROADS_UL && this->data[ col ][ row ] <= CROSSROADS_LR;
		}

	private:
		char **data = NULL;	// dane, tablica 2d, info o typie plytki
		char **taken = NULL;	// ile aut zajmuje dana plytke
		int width, height;	// wymiary, ile kolumn i wierszy

		std::vector< std::pair< int, int > > spawners;	// plytki, ktore generuja kolejne samochody

		std::map< std::pair< int, int >, Light* > lights;
};

#endif // _TILE_MAP_H_
