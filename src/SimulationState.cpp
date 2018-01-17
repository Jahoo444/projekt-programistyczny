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

	this->nextRespawnTime = SDL_GetTicks() + rand() % ( this->maxRespawnTime - this->minRespawnTime ) + this->minRespawnTime;

	int w = map->getWidth(), h = map->getHeight();
	int s = TileMap::TILE_SIZE;

	this->camera = new Camera( w / 2 * s, h / 2 * s, 1.0f, w * s, h * s);
}

void SimulationState::setDensity( enum MenuState::TRAFFIC_DENSITIES density )
{
	switch( density )
	{
		case MenuState::TRAFFIC_DENSITIES::LOW:
			minRespawnTime = 3000;
			maxRespawnTime = 7000;
			break;
		
		case MenuState::TRAFFIC_DENSITIES::MEDIUM:
			minRespawnTime = 2000;
			maxRespawnTime = 5000;
			break;
		
		case MenuState::TRAFFIC_DENSITIES::HIGH:
			minRespawnTime = 1000;
			maxRespawnTime = 3000;
			break;
		
		default:
			minRespawnTime = 1000;
			maxRespawnTime = 3000;
			break;
	}
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
		car->update(cars);

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

		this->nextRespawnTime = SDL_GetTicks() + rand() % ( this->maxRespawnTime - this->minRespawnTime ) + this->minRespawnTime;
	}
}

void SimulationState::render( Renderer *renderer )
{
	renderer->render( this );
}

traffic_state SimulationState::getTrafficState() {
	double wait_time {0};
	double max_wait_time {0};
	long unsigned int car_count {0};
	intersection_node up    {Light::DIRECTIONS::UP,    0, 0, this->map->getColor(Light::DIRECTIONS::UP), this->map->getCommandsSize(Light::DIRECTIONS::UP)};
	intersection_node down  {Light::DIRECTIONS::DOWN,  0, 0, this->map->getColor(Light::DIRECTIONS::DOWN), this->map->getCommandsSize(Light::DIRECTIONS::DOWN)};
	intersection_node right {Light::DIRECTIONS::RIGHT, 0, 0, this->map->getColor(Light::DIRECTIONS::RIGHT), this->map->getCommandsSize(Light::DIRECTIONS::RIGHT)};
	intersection_node left  {Light::DIRECTIONS::LEFT,  0, 0, this->map->getColor(Light::DIRECTIONS::LEFT), this->map->getCommandsSize(Light::DIRECTIONS::LEFT)};
	for(const auto& car:cars) {
		if(!car->passedCrossroad()) {
			wait_time += car->elpased_time();
			if(car->elpased_time() > max_wait_time)
				max_wait_time = car->elpased_time();
			car_count++;
			switch( car->getDirection() )
			{
			case Car::DIRECTIONS::UP:
					std::get<1>(up)++;
					std::get<2>(up)+=car->elpased_time();
					break;

				case Car::DIRECTIONS::DOWN:
					std::get<1>(down)++;
					std::get<2>(down)+=car->elpased_time();
					break;

				case Car::DIRECTIONS::RIGHT:
					std::get<1>(right)++;
					std::get<2>(right)+=car->elpased_time();
					break;

				case Car::DIRECTIONS::LEFT:
					std::get<1>(left)++;
					std::get<2>(left)+=car->elpased_time();
					break;

				default:
					break;
			}
		}
	}

	traffic_state t {
		car_count,
		wait_time,
		max_wait_time,
		{up, down, right, left}
	};


	return t;
}

void SimulationState::addLightCommand(std::vector<command> cmd_vec)
{
	for(auto& cmd : cmd_vec)
	this->map->addLightCommand(cmd);
}

void SimulationState::addLightCommand(command& c)
{
	this->map->addLightCommand(c);
}

void SimulationState::clearLightCommands(Light::DIRECTIONS dir)
{
	this->map->clearLightCommands(dir);
}
