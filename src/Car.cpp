#include "Car.h"
#include "TileMap.h"
#include "Light.h"

#include <stdlib.h>
#include <iostream>
#include <chrono>

Car::Car( TileMap *map, int column, int row ) : creation_time{std::chrono::system_clock::now()}
{
	this->map = map;

	int ts = TileMap::TILE_SIZE;
	this->x = column * ts + ts / 2;
	this->y = row * ts + ts / 2;

	this->remove = false;

	map->takeTile( column, row );

	char **data = map->getData();
	char dir = data[ column ][ row ];

	switch( dir )
	{
		case TileMap::TILE_TYPES::ROAD_UP:
			this->dir = UP;
			break;

		case TileMap::TILE_TYPES::ROAD_DOWN:
			this->dir = DOWN;
			break;

		case TileMap::TILE_TYPES::ROAD_RIGHT:
			this->dir = RIGHT;
			break;

		case TileMap::TILE_TYPES::ROAD_LEFT:
			this->dir = LEFT;
			break;

		default:
			break;
	}

	this->turnDir = ( enum TURN_DIRECTIONS ) ( rand() % ( int ) NUM_TURN_DIRECTIONS );

	this->texture = rand() % 10;
}

void Car::update(const std::vector< Car* > cars)
{
	int ts = TileMap::TILE_SIZE, mw = map->getWidth(), mh = map->getHeight();
	int col = this->x / ts, row = this->y / ts;

	this->map->leaveTile( col, row );

	if( col < 0 || col >= mw || row < 0 || row >= mh )
	{
		this->remove = true;

		return;
	}

	char **data = this->map->getData();
	char dir = data[ col ][ row ];

	if( dir < TileMap::TILE_TYPES::ROAD_UP || dir > TileMap::TILE_TYPES::CROSSROADS_LR )
	{
		this->remove = true;

		return;
	}

	float dx, dy;

	switch( dir )
	{
		case TileMap::TILE_TYPES::ROAD_UP:
			dx = 0.0f, dy = -SPEED;
			break;

		case TileMap::TILE_TYPES::ROAD_DOWN:
			dx = 0.0f, dy = SPEED;
			break;

		case TileMap::TILE_TYPES::ROAD_LEFT:
			dx = -SPEED, dy = 0.0f;
			break;

		case TileMap::TILE_TYPES::ROAD_RIGHT:
			dx = SPEED, dy = 0.0f;
			break;

		default:
			dx = ( this->dir == RIGHT ) ? SPEED : ( this->dir == LEFT ) ? -SPEED : 0.0f;
			dy = ( this->dir == UP ) ? -SPEED : ( this->dir == DOWN ) ? SPEED : 0.0f;
			break;
	}

	Light::COLORS color;
	Light::DIRECTIONS lightDir;

	switch( this->dir )
	{
		case UP:
			color = this->map->getColor( ( this->x + dx ) / ts + 1, ( this->y + dy ) / ts );
			lightDir = this->map->getLightDirection( ( this->x + dx ) / ts + 1, ( this-> y + dy ) / ts );
			break;

		case DOWN:
			color = this->map->getColor( ( this->x + dx ) / ts - 1, ( this-> y + dy ) / ts );
			lightDir = this->map->getLightDirection( ( this->x + dx ) / ts - 1, ( this-> y + dy ) / ts );
			break;

		case RIGHT:
			color = this->map->getColor( ( this->x + dx ) / ts, ( this-> y + dy ) / ts + 1 );
			lightDir = this->map->getLightDirection( ( this->x + dx ) / ts , ( this-> y + dy ) / ts + 1 );
			break;

		case LEFT:
			color = this->map->getColor( ( this->x + dx ) / ts, ( this-> y + dy ) / ts - 1 );
			lightDir = this->map->getLightDirection( ( this->x + dx ) / ts, ( this-> y + dy ) / ts - 1 );
			break;

		default:
			break;
	}

	if( color != Light::COLORS::GREEN && ( int ) this->dir == ( int ) lightDir )
		dx = dy = 0.0f;

	/*int vfront = ( this->dir == UP ) ? -this->width / 2 : ( this->dir == DOWN ) ? this->width / 2 : 0;
	int hfront = ( this->dir == LEFT ) ? -this->height / 2 : ( this->dir == RIGHT ) ? this->height / 2 : 0;
	if( this->map->tileTaken( ( this->x + dx + hfront ) / ts, ( this->y + dy + vfront ) / ts ) )
		dx = dy = 0.0f;*/

	for(const auto& c: cars) {
		if(this != c && isFrontCollision(c, 30.0)) {
			dx = dy = 0.0f;
		}
	}

	this->dx = dx, this->dy = dy;

	this->x += dx, this->y += dy;

	col = this->x / ts, row = this->y / ts;
	this->map->takeTile( col, row );

	makeTurn();
}

bool Car::isFrontCollision(Car *c, float distance) {

	bool collision = false;

	std::pair< float, float > pos1 = this->getPosition();
	std::pair< float, float > pos2 = c->getPosition();

	int width1 = spriteWidth(), width2 = c->spriteWidth();
	int height1 = spriteHeight(), height2 = c->spriteHeight();

	if(this->dir == UP || this->dir == DOWN)
		height1 += distance;
	else if(this->dir == LEFT || this->dir == RIGHT)
		width1 += distance;

	float w = 0.5 * (width1 + width2);
	float h = 0.5 * (height1 + height2);

	float dx = pos2.first - pos1.first;
	float dy = pos2.second - pos1.second;

	enum DIRECTIONS collision_dir;

	if(abs(dx) <= w && abs(dy) <= h) {

		float wy = w * dy;
	    float hx = h * dx;

	    if(wy > hx) {
	        if(wy > -hx) {
				collision_dir = DOWN;
			}
	        else if(wy <= -hx) {
				collision_dir = LEFT;
			}
		}
	    else {
	        if(wy > -hx) {
				collision_dir = RIGHT;
			}
	        else if(wy <= -hx) {
				collision_dir = UP;
			}
		}

		if(this->dir == collision_dir)
			collision = true;
	}

	return collision;
}

void Car::makeTurn()
{
	if( this->turnDir == NO_TURN )
		return;

	int ts = TileMap::TILE_SIZE;
	int col = this->x / ts, row = this->y / ts;
	if( this->map->onCrossroads( this->x / ts, this->y / ts ) )
	{
		is_leaving = true;
		switch( this->turnDir )
		{
			case TURN_RIGHT:
				if( abs( ( int ) this->x % ts - ts / 2 ) < SPEED && ( this->dir == RIGHT || this->dir == LEFT ) )
				{
					this->turnDir = NO_TURN;
					this->dir = ( this->dir == RIGHT ) ? DOWN : UP;
				}
				else if( abs( ( int ) this->y % ts - ts / 2 ) < SPEED && ( this->dir == UP || this->dir == DOWN ) )
				{
					this->turnDir = NO_TURN;
					this->dir = ( this->dir == UP ) ? RIGHT : LEFT;
				}
				break;

			case TURN_LEFT:
				{
					char **data = this->map->getData();
					char dir = data[ col ][ row ];

					switch( dir )
					{
						case TileMap::TILE_TYPES::CROSSROADS_UL:
							if( this->dir == LEFT )
							{
								if( abs( ( int ) this->x % ts - ts / 2 ) < SPEED )
								{
									this->turnDir = NO_TURN;
									this->dir = DOWN;
								}
							}
							break;

						case TileMap::TILE_TYPES::CROSSROADS_LL:
							if( this->dir == DOWN )
							{
								if( abs( ( int ) this->y % ts - ts / 2 ) < SPEED )
								{
									this->turnDir = NO_TURN;
									this->dir = RIGHT;
								}
							}
							break;

						case TileMap::TILE_TYPES::CROSSROADS_UR:
							if( this->dir == UP )
							{
								if( abs( ( int ) this->y % ts - ts / 2 ) < SPEED )
								{
									this->turnDir = NO_TURN;
									this->dir = LEFT;
								}
							}
							break;

						case TileMap::TILE_TYPES::CROSSROADS_LR:
							if( this->dir == RIGHT )
							{
								if( abs( ( int ) this->x % ts - ts / 2 ) < SPEED )
								{
									this->turnDir = NO_TURN;
									this->dir = UP;
								}
							}
							break;

						default:
							// woot?
							break;
					}
				}
				break;

			default:	//
				break;
		}
	}
}

double Car::elpased_time()
{
	auto actual_time = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = actual_time-creation_time;
	return elapsed_seconds.count();
}

bool Car::passedCrossroad()
{
	int ts = TileMap::TILE_SIZE;
	return !this->map->onCrossroads( this->x / ts, this->y / ts ) && is_leaving;
}
