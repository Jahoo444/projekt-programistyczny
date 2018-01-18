#include "Simulation.h"

#include "Light.h"
#include "traffic_state.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"
#include <fstream>
#include <chrono>
#include <limits>
#include <time.h>

std::ofstream res_pre("results_with_precognition.txt", std::ios::binary);
std::ofstream res("results.txt", std::ios::binary);
auto start = std::chrono::system_clock::now();

Uint32 info_callback_pre(Uint32 interval, void *param)
{
	auto state = static_cast<SimulationState*>(param);
	auto ts = state->getTrafficState();
	//std::cout<<ts<<std::endl;

	intflow::Session<int,int,double,int,int> session;
	intflow::NewSession(0, &session);

	session.ConstraintsMin(std::make_tuple(Light::DIRECTIONS::UP,0,0.0,0,0));
	session.ConstraintsMax(std::make_tuple(Light::DIRECTIONS::RIGHT,
	                        std::numeric_limits<int>::max(),
	                        std::numeric_limits<double>::max(),
	                        Light::COLORS::RED,
	                        std::numeric_limits<int>::max()));

	for(auto& it : ts.intensity )
	{
		session.Extend(it);
	}

	auto results = session.Run();

	state->addLightCommand(results);


	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	res_pre<<elapsed_seconds.count()<<"\t"<<ts.avg_wait_time<<"\t"<<ts.max_wait_time<<std::endl;



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

Uint32 info_callback(Uint32 interval, void *param)
{
	auto state = static_cast<SimulationState*>(param);
	auto ts = state->getTrafficState();
	std::cout<<ts<<std::endl;

	std::vector<command> cmd {command::command_change_light(Light::DIRECTIONS::UP),
							  command::command_change_light(Light::DIRECTIONS::DOWN),
							  command::command_change_light(Light::DIRECTIONS::LEFT),
							  command::command_change_light(Light::DIRECTIONS::RIGHT),
							  command::command_wait(Light::DIRECTIONS::UP, 8000),
							  command::command_wait(Light::DIRECTIONS::DOWN, 8000),
							  command::command_wait(Light::DIRECTIONS::LEFT, 8000),
							  command::command_wait(Light::DIRECTIONS::RIGHT, 8000)};
	state->addLightCommand(cmd);

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
	res<<elapsed_seconds.count()<<"\t"<<ts.avg_wait_time<<"\t"<<ts.max_wait_time<<std::endl;



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

void Simulation::init(bool use_precognition)
{
	this->use_precognition = use_precognition;
	srand( 1 );
	this->renderer.init();

	this->state = new MenuState( this );
	this->state->init();
	/*
	this->state = new SimulationState();
	this->state->init();
	if(use_precognition)
		timerID = SDL_AddTimer( 1000, info_callback_pre, this->state);
	else
		timerID = SDL_AddTimer( 1000, info_callback, this->state);
	*/
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

void Simulation::startSimulation( int crossroads, enum MenuState::TRAFFIC_DENSITIES density )
{
	SimulationState *state = new SimulationState();
	state->init( crossroads );
	state->setDensity( density );
	this->state = state;

	if(use_precognition)
		timerID = SDL_AddTimer( 1000, info_callback_pre, this->state);
	else
		timerID = SDL_AddTimer( 1000, info_callback, this->state);
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
			res.close();
			res_pre.close();
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
