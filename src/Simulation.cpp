#include "Simulation.h"

#include <time.h>

Simulation::Simulation()
{
}

void Simulation::init()
{
	srand( time( NULL ) );
	this->renderer.init();
	this->state = new SimulationState();
	this->state->init();
}

void Simulation::run()
{
	bool running = true;

	while( running )
	{
		Uint32 timeStarted = SDL_GetTicks();	// czas rozpoczecia klatki

		running = handleEvents();
		update();
		render();

		Uint32 timeElapsed = SDL_GetTicks() - timeStarted;

		// odczekac do nastepnej klatki, by bylo w miare plynne 60 fpsow
		if( timeElapsed < MILLIS_PER_FRAME )
		{
			SDL_Delay( MILLIS_PER_FRAME - timeElapsed );
		}
	}
}

bool Simulation::handleEvents()
{
	SDL_Event event;
	bool running = true;

	while( running && SDL_PollEvent( &event ) )
	{
		if( event.type == SDL_QUIT )
			running = false;
		else
			running = this->state->handleEvent( &event );
	}

	return running;
}

void Simulation::update()
{
	this->state->update();
}

void Simulation::render()
{
	this->state->render( &( this->renderer ) );
}
