#include "MenuState.h"
#include "SimulationState.h"
#include "Simulation.h"

MenuState::MenuState( Simulation *simulation )
{
	this->simulation = simulation;
}

void MenuState::init()
{
	this->currentOption = START;
	this->density = HIGH;
}

bool MenuState::select()
{
	bool running = true;
	
	switch( this->currentOption )
	{
		case START:
			{
				this->simulation->startSimulation( this->crossroads, this->density );
				delete this;
			}
			break;
		
		case QUIT:
			running = false;
			break;
		
		default:
			break;
	}
	
	return running;
}

void MenuState::update()
{
}

void MenuState::render( Renderer *renderer )
{
	renderer->render( this );
}

bool MenuState::handleEvent( SDL_Event *event )
{
	bool running = true;

	if( event->type == SDL_KEYDOWN )
	{
		switch( event->key.keysym.sym )
		{
			case SDLK_ESCAPE:
				running = false;
				break;

			case SDLK_UP:
				if( this->currentOption == START )
					this->currentOption = QUIT;
				else
					this->currentOption = ( enum OPTIONS ) ( ( int ) this->currentOption - 1 );
				break;

			case SDLK_DOWN:
				this->currentOption = ( enum OPTIONS ) ( ( ( int ) this->currentOption + 1 ) % ( int ) NUM_OPTIONS );
				break;

			case SDLK_LEFT:
				if( this->currentOption == CHOOSE_DENSITY )
				{
					if( this->density == LOW )
						this->density = HIGH;
					else
						this->density = ( enum TRAFFIC_DENSITIES ) ( ( ( int ) this->density - 1 ) % ( int ) NUM_TRAFFIC_DENSITIES );
				}
				else if( this->currentOption == CHOOSE_CROSSROADS )
				{
					this->crossroads--;
					if( this->crossroads < 0 )
						this->crossroads = NUM_CROSSROADS - 1;
				}
				break;

			case SDLK_RIGHT:
				if( this->currentOption == CHOOSE_DENSITY )
					this->density = ( enum TRAFFIC_DENSITIES ) ( ( ( int ) this->density + 1 ) % ( int ) NUM_TRAFFIC_DENSITIES );
				else if( this->currentOption == CHOOSE_CROSSROADS )
					this->crossroads = ( this->crossroads + 1 ) % NUM_CROSSROADS;
				break;
			
			case SDLK_RETURN:
				running = select();
				break;

			default:
				break;
		}
	}

	return running;
}