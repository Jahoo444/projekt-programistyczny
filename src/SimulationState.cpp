#include "SimulationState.h"
#include "Renderer.h"

#include <tuple>

SimulationState::SimulationState()
{
}

void SimulationState::init()
{
	this->map = new TileMap();
	this->map->load( "config/map" );

	std::vector< std::pair< int, int > > *spawners = this->map->getSpawners();

	for( int i = 0; i < spawners->size(); i++ )
	{
		auto pos = ( *spawners )[ i ];
		int col = std::get< 0 >( pos ), row = std::get< 1 >( pos );
		Car *car = new Car( this->map, col, row );
		this->cars.push_back( car );
	}

	this->nextRespawnTime = SDL_GetTicks() + rand() % ( MAX_RESPAWN_TIME - MIN_RESPAWN_TIME ) + MIN_RESPAWN_TIME;

	int w = map->getWidth(), h = map->getHeight();
	int s = TileMap::TILE_SIZE;

	this->camera = new Camera( w / 2 * s, h / 2 * s, 1.0f, w * s, h * s);
}

bool SimulationState::handleEvent( SDL_Event *event )
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
				this->camera->move( 0.0f, -this->camera->MOVE_SPEED );
				break;

			case SDLK_DOWN:
				this->camera->move( 0.0f, this->camera->MOVE_SPEED );
				break;

			case SDLK_LEFT:
				this->camera->move( -this->camera->MOVE_SPEED, 0.0f );
				break;

			case SDLK_RIGHT:
				this->camera->move( this->camera->MOVE_SPEED, 0.0f );
				break;

			case SDLK_z:
				this->camera->zoomin();
				break;

			case SDLK_x:
				this->camera->zoomout();
				break;

			default:
				break;
		}
	}

	return running;
}

void SimulationState::update()
{
	for( int i = 0; i < this->cars.size(); i++ )
	{
		Car *car = this->cars[ i ];
		car->update();

		if( car->shouldRemove() )
		{
			this->cars.erase( this->cars.begin() + i );
			delete car;
			i--;
		}
	}

	this->map->update();

	Uint32 time = SDL_GetTicks();

	if( time >= this->nextRespawnTime )
	{
		std::vector< std::pair< int, int > > *spawners = this->map->getSpawners();

		int roll = rand() % spawners->size();

		auto pos = ( *spawners )[ roll ];
		int col = std::get< 0 >( pos ), row = std::get< 1 >( pos );
		Car *car = new Car( this->map, col, row );
		this->cars.push_back( car );

		this->nextRespawnTime = SDL_GetTicks() + rand() % ( MAX_RESPAWN_TIME - MIN_RESPAWN_TIME ) + MIN_RESPAWN_TIME;
	}
}

void SimulationState::render( Renderer *renderer )
{
	renderer->render( this );
}
