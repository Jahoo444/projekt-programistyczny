#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_

#include <SDL.h>

#include "State.h"

class Simulation;

class MenuState : public State
{
	public:
		static const int NUM_CROSSROADS = 3;
		static const int NUM_MAPS = 3;
		
		// wybor menu
		enum OPTIONS
		{
			START,
			CHOOSE_DENSITY,
			CHOOSE_CROSSROADS,
			QUIT,
			
			NUM_OPTIONS
		};
		
		// natezenia ruchu: niskie, srednie, wysokie
		enum TRAFFIC_DENSITIES
		{
			LOW,
			MEDIUM,
			HIGH,
			
			NUM_TRAFFIC_DENSITIES
		};
		
		const char *MAP_PATHS[ NUM_MAPS ]
		{
			"config/map1",
			"config/map2",
			"config/map3"
		};
		
		MenuState( Simulation *simulation );
		
		void init();
		void update();
		void render( Renderer *renderer );

		bool handleEvent( SDL_Event *event );
		
		enum OPTIONS getCurrentOption() { return this->currentOption; }
		enum TRAFFIC_DENSITIES getTrafficDensity() { return this->density; }
		
		int getCrossroads() { return this->crossroads; }
	
	private:
		bool select();
		
		Simulation *simulation = NULL;
		
		enum OPTIONS currentOption;
		enum TRAFFIC_DENSITIES density;
		int crossroads = 0;
};

#endif // _MENU_STATE_H_