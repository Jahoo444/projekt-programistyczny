/*
	Glowna aplikacja
	Sterowanie stanami symulacji
*/

#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include <SDL.h>

#include "Renderer.h"
#include "SimulationState.h"
#include "MenuState.h"

class Simulation
{
	public:
		static const int FPS = 60;	// klatki animacji/aktualizacji stanu na sekunde
		static const int MILLIS_PER_FRAME = 1000 / FPS;	// ile milisekund na klatke

		Simulation();

		void init(bool use_precognition = true);
		void run();
		
		void startSimulation( int crossroads, enum MenuState::TRAFFIC_DENSITIES density );

	private:
		void update();
		void render();

		bool handleEvents();

		Renderer renderer;
		State *state = NULL;

		SDL_TimerID timerID;
		
		bool use_precognition;
};

#endif // _SIMULATION_H_
