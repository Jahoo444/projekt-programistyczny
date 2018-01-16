#include "Simulation.h"

#include "traffic_state.h"
#include <time.h>

constexpr bool SHOW_TRAFFIC_INFO { true };

Uint32 info_callback(Uint32 interval, void *param)
{
	auto state = static_cast<SimulationState*>(param);
	auto ts = state->getTrafficState();
	std::cout<<ts<<std::endl;

	/* Input Traffic Precognition Here*/

	SDL_Event event;
    SDL_UserEvent userevent;
    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return(interval);
}

Simulation::Simulation()
{
}

void Simulation::init()
{
	srand( time( NULL ) );
	this->renderer.init();
	this->state = new SimulationState();
	this->state->init();
	if(SHOW_TRAFFIC_INFO)
	timerID = SDL_AddTimer( 5 * 1000, info_callback, this->state);
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
		{
			running = false;
			SDL_RemoveTimer( timerID );
		}
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
