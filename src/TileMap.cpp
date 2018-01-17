#include "TileMap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

TileMap::TileMap()
{
	this->width = this->height = 0;
}

void TileMap::load( const char *path )
{
	if( this->data != NULL )
	{
		for( int i = 0; i < this->height; i++ )
			delete [] this->data[ i ];

		delete [] this->data;
	}

	while( !this->spawners.empty() )
		this->spawners.pop_back();

	FILE *file = fopen( path, "r" );

	if( file == NULL )
	{
		printf( "Could not load map file %s: %s\n", path, strerror( errno ) );

		exit( -1 );
	}

	int width, height;

	fscanf( file, "%d %d\n", &width, &height );

	this->width = width, this->height = height;

	char **data;

	data = new char * [ this->width ];

	for( int i = 0; i < this->width; i++ )
		data[ i ] = new char[ this->height ];

	for( int row = 0; row < this->height; row++ )
	{
		for( int column = 0; column < this->width; column++ )
		{
			int val;
			fscanf( file, "%d", &val );
			data[ column ][ row ] = val;
		}
	}

	this->data = data;

	char **taken;

	taken = new char * [ this->width ];

	for( int i = 0; i < this->width; i++ )
		taken[ i ] = new char[ this->height ];

	for( int row = 0; row < this->height; row++ )
	{
		for( int column = 0; column < this->width; column++ )
		{
			taken[ column ][ row ] = 0;
		}
	}

	this->taken = taken;

	int numlights;
	fscanf( file, "%d", &numlights );

	Uint32 time = SDL_GetTicks();

	for( int i = 0; i < numlights; i++ )
	{
		int dir, row, column, color;
		fscanf( file, "%d %d %d %d\n", &dir, &column, &row, &color );
		this->lights[ std::make_pair( column, row ) ] = new Light( ( enum Light::DIRECTIONS ) dir, ( enum Light::COLORS ) color, time );
	}

	int numspawners;
	fscanf( file, "%d", &numspawners );

	for( int i = 0; i < numspawners; i++ )
	{
		int row, column;
		fscanf( file, "%d %d\n", &column, &row );
		this->spawners.push_back( std::make_pair( column, row ) );
	}

	fclose( file );
}

void TileMap::update()
{
	Uint32 time = SDL_GetTicks();
	for( auto const & item : this->lights )
	{
		item.second->update();
		auto cmd = item.second->getCommand();
		switch(cmd.getCommandType())
		{
		case command::CHANGE_LIGHT:
			item.second->switchColor();
			item.second->popFirstCommand();
			break;
		case command::WAIT:
			if(time - item.second->getSwitchTime() >= cmd.getTime())
			{
				item.second->popFirstCommand();
			}
			break;
		default :
			printf("UNKNOWN COMMAND\n");
			break;
		}
	}
}

Light::COLORS TileMap::getColor( int col, int row )
{
	std::map< std::pair< int, int >, Light* >::iterator it = this->lights.find( std::make_pair( col, row ) );

	if( it != this->lights.end() )
	{
		return it->second->getColor();
	}
	else
		return Light::COLORS::GREEN;
}

Light::COLORS TileMap::getColor( Light::DIRECTIONS dir )
{
	for(const auto& light:lights){
		if(light.second->getDirection() == dir)
			return light.second->getColor();
	}
	return Light::COLORS::NUM_COLORS;
}

int TileMap::getCommandsSize( Light::DIRECTIONS dir )
{
	for(const auto& light:lights){
		if(light.second->getDirection() == dir)
			return light.second->getCommandSize();
	}
	return 0;
}

Light::DIRECTIONS TileMap::getLightDirection( int col, int row )
{
	std::map< std::pair< int, int >, Light* >::iterator it = this->lights.find( std::make_pair( col, row ) );

	if( it != this->lights.end() )
	{
		return it->second->getDirection();
	}
	else
		return Light::DIRECTIONS::NONE;
}

void TileMap::addLightCommand(command& c)
{
	auto dir = c.getLightDirection();
	for(const auto& light:lights){
		if(light.second->getDirection() == dir)
			light.second->addCommand(c);
	}
}

void TileMap::clearLightCommands(Light::DIRECTIONS dir)
{
	for(const auto& light:lights){
		if(light.second->getDirection() == dir)
			light.second->clearCommands();
	}
}
