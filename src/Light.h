/*
	Swiatla
*/

#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <SDL.h>

class Light
{
	public:
		static const int SWITCH_DURATION = 1000;	// jak dlugo pali sie zolte
		
		enum COLORS
		{
			GREEN,
			YELLOW,
			RED,
			
			NUM_COLORS
		};
		
		// kierunki poruszania sie, ktorych dotyczy sygnalizacja
		enum DIRECTIONS
		{
			UP,
			DOWN,
			LEFT,
			RIGHT,
			
			NONE,
			
			NUM_DIRECTIONS
		};
		
		Light( enum DIRECTIONS dir, enum COLORS color, Uint32 startTime )
		{
			this->dir = dir;
			this->color = color;
			this->startTime = startTime;
			this->switchTime = startTime;
		}
		
		void switchColor()
		{
			switch( this->color )
			{
				case GREEN:
					this->color = YELLOW;
					this->switchTime = SDL_GetTicks();
					this->targetColor = RED;
					break;
				
				case RED:
					this->color = YELLOW;
					this->switchTime = SDL_GetTicks();
					this->targetColor = GREEN;
					break;
				
				case YELLOW:	// hmm
					break;
				
				default:
					break;
			}
		}
		
		void update()
		{
			if( this->color == YELLOW )
			{
				Uint32 time = SDL_GetTicks();
				
				if( time - this->switchTime >= SWITCH_DURATION )
				{
					this->color = this->targetColor;
				}
			}
		}
		
		enum DIRECTIONS getDirection() { return this->dir; }
		enum COLORS getColor() { return this->color; }
		
		Uint32 getStartTime() { return this->startTime; }
		Uint32 getSwitchTime() { return this->switchTime; }
		
		int spriteWidth() { if( this->dir == UP || this->dir == DOWN ) return width; else return height; }	// szerokosc grafiki
		int spriteHeight() { if( this->dir == UP || this->dir == DOWN ) return height; else return width; }	// wysokosc grafiki
		
		static const int width = 26, height = 52;
	
	private:
		enum COLORS color;	// aktualne swiatlo
		enum COLORS targetColor;	// na jakie swiatlo zmienic
		Uint32 startTime;	// czas startu dzialania
		Uint32 switchTime;	// czas ostatniej zmiany swiatel
		
		enum DIRECTIONS dir;
};

#endif // _LIGHT_H_