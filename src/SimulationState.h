/*
	Stan symulacji
	To tu rozgrywa sie cala akcja
*/

#ifndef _SIMULATION_STATE_H_
#define _SIMULATION_STATE_H_

#include <SDL.h>

#include "State.h"
#include "TileMap.h"
#include "Car.h"
#include "Camera.h"
#include "traffic_state.h"
#include "command.h"
#include "MenuState.h"

class Renderer;

class SimulationState : public State
{
	public:
		//static const int MIN_RESPAWN_TIME = 1000;
		//static const int MAX_RESPAWN_TIME = 3000;

		SimulationState();

		void init( int crossroads );
		void update();
		void render( Renderer *renderer );

		bool handleEvent( SDL_Event *event );

		TileMap *getMap() { return this->map; }
		std::vector< Car* > *getCars() { return &( this->cars ); }

		Camera *getCamera() { return this->camera; }
		
		void setDensity( enum MenuState::TRAFFIC_DENSITIES density );

		traffic_state getTrafficState();

		void addLightCommand(command& c);
		void addLightCommand(std::vector<command> cmd_vec);
		void clearLightCommands(Light::DIRECTIONS dir);

	private:
		TileMap *map = NULL;	// mapa ze skrzyzowaniami
		std::vector< Car* > cars;	// samochody

		Camera *camera;

		Uint32 nextRespawnTime;
		
		int minRespawnTime = 1000;
		int maxRespawnTime = 3000;
};

#endif // _SIMULATIONS_STATE_H_
