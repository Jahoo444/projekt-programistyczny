/*
	Samochody
*/

#ifndef _CAR_H_
#define _CAR_H_

#include "TileMap.h"
#include <chrono>

class Car
{
	public:
		// kierunki poruszania sie
		enum DIRECTIONS
		{
			UP,
			DOWN,
			LEFT,
			RIGHT,

			NUM_DIRECTIONS
		};

		// kierunki skretu
		enum TURN_DIRECTIONS
		{
			NO_TURN,
			TURN_LEFT,
			TURN_RIGHT,

			NUM_TURN_DIRECTIONS
		};

		float SPEED = 1.5f;	// szybkosc

		Car( TileMap *map, int column, int row );

		void update(const std::vector< Car* > cars);

		void makeTurn(const std::vector< Car* > cars);	// skrecanie auta (o ile ma taki zamiar i moze wykonac manewr)

		std::pair< float, float > getPosition() { return std::make_pair( this->x, this-> y ); }

		bool shouldRemove() { return this->remove; }

		int spriteWidth() { if( this->dir == UP || this->dir == DOWN ) return width; else return height; }	// szerokosc grafiki
		int spriteHeight() { if( this->dir == UP || this->dir == DOWN ) return height; else return width; }	// wysokosc grafiki

		int whichTexture() { return this->texture; }

		bool isFrontCollision(Car *c, float distance);

		enum DIRECTIONS getDirection() { return this->dir; }

		static const int width = 28, height /* length */ = 52;	// wymiary

		double elpased_time();

		bool passedCrossroad();

	private:
		TileMap *map;
		float x, y;	// pozycja
		float dx, dy;	// predkosc

		enum DIRECTIONS dir;	// kierunek poruszania sie
		enum TURN_DIRECTIONS turnDir;	// kierunek skretu

		int texture;

		bool remove;	// czy usunac
		bool is_leaving {false};

		std::chrono::time_point<std::chrono::system_clock> creation_time;
};

#endif // _CAR_H_
