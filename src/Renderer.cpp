#include "Renderer.h"
#include "Car.h"
#include "Light.h"

#include <vector>
#include <tuple>
#include <map>

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
	SDL_DestroyWindow( this->window );
	SDL_Quit();
}

void Renderer::init()
{
	this->width = DEFAULT_WIDTH;
	this->height = DEFAULT_HEIGHT;
	this->fullscreen = DEFAULT_FULLSCREEN;

	FILE *file = fopen( "config/config.txt", "r" );

	int w, h, fs;

	fscanf( file, "%d %d\n", &w, &h );
	fscanf( file, "%d\n", &fs );

	this->width = w, this->height = h;
	this->fullscreen = fs;

	fclose( file );

	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );

		exit( -1 );
	}
	else
	{
		this->window = SDL_CreateWindow(	"Traffic simulator",
											SDL_WINDOWPOS_UNDEFINED,
											SDL_WINDOWPOS_UNDEFINED,
											this->width,
											this->height,
											this->fullscreen );

		if( this->window == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );

			exit( -1 );
		}
		else
		{
			this->screen = SDL_GetWindowSurface( this->window );
		}
	}

	loadResources();
}

void Renderer::loadResources()
{
	this->resources.textures[ 0 ] = SDL_LoadBMP( "res/textures/out.bmp" );
	this->resources.textures[ 1 ] = SDL_LoadBMP( "res/textures/verrgt.bmp" );
	this->resources.textures[ 2 ] = SDL_LoadBMP( "res/textures/verlft.bmp" );
	this->resources.textures[ 3 ] = SDL_LoadBMP( "res/textures/horuppr.bmp" );
	this->resources.textures[ 4 ] = SDL_LoadBMP( "res/textures/horlwr.bmp" );
	this->resources.textures[ 5 ] = SDL_LoadBMP( "res/textures/upprlft.bmp" );
	this->resources.textures[ 6 ] = SDL_LoadBMP( "res/textures/lwrlft.bmp" );
	this->resources.textures[ 7 ] = SDL_LoadBMP( "res/textures/upprrgt.bmp" );
	this->resources.textures[ 8 ] = SDL_LoadBMP( "res/textures/lwrrgt.bmp" );

	for( int i = 0; i < Resources::NUM_CARS; i++ )
	{
		char path[] = "res/cars/carXY.bmp";
		path[ 12 ] = i + '0';

		for( int j = 0; j < Resources::NUM_DIRECTIONS; j++ )
		{
			path[ 13 ] = j + '1';
			this->resources.carTextures[ i ][ j ] = SDL_LoadBMP( path );
			SDL_SetColorKey( this->resources.carTextures[ i ][ j ], SDL_TRUE, SDL_MapRGB( this->screen->format, 0, 255, 255 ) );
		}
	}

	for( int i = 0; i < Resources::NUM_COLORS; i++ )
	{
		for( int j = 0; j < Resources::NUM_DIRECTIONS; j++ )
		{
			char path[] = "res/lights/lightXY.bmp";
			path[ 16 ] = i + '1', path[ 17 ] = j + '1';
			this->resources.lightTextures[ i ][ j ] = SDL_LoadBMP( path );
		}
	}
}

void Renderer::render( SimulationState *state )
{
	TileMap *map = state->getMap();
	int width = map->getWidth(), height = map->getHeight();
	char **data = map->getData();

	Camera *camera = state->getCamera();
	auto cpos = camera->getPosition();
	int cx = std::get< 0 >( cpos ), cy = std::get< 1 >( cpos );

	float scale = camera->getZoom();

	SDL_FillRect( this->screen, NULL, SDL_MapRGB( this->screen->format, 0, 0, 0 ) );

	const int s = TileMap::TILE_SIZE;

	for( int row = 0; row < height; row++ )
	{
		for( int column = 0; column < width; column++ )
		{
			int which = data[ column ][ row ];

			if( which < 0 || which > TileMap::TILE_TYPES::CROSSROADS_LR )
				which = 0;

			SDL_Surface *surf = this->resources.textures[ which ];

			SDL_Rect rect = {	( int ) ( scale * ( column * s - cx ) + this->width / 2 ),
								( int ) ( scale * ( row * s - cy ) + this->height / 2 ),
								( int ) ( scale * s ),
								( int ) ( scale * s ) };

			SDL_BlitScaled( surf, NULL, this->screen, &rect );
		}
	}

	std::map< std::pair< int, int >, Light* > *lights = map->getLights();

	for( auto const & item : *lights )
	{
		auto pos = item.first;
		int column = std::get< 0 >( pos ), row = std::get< 1 >( pos );

		Light *light = item.second;

		int w = light->spriteWidth(), h = light->spriteHeight();

		SDL_Rect rect = {	( int ) ( scale * ( column * s + s / 2 - w / 2 - cx ) + this->width / 2 ),
							( int ) ( scale * ( row * s + s / 2 - h / 2 - cy ) + this-> height / 2 ),
							( int ) ( scale * w ),
							( int ) ( scale * h ) };

		int color = ( int ) light->getColor();
		int dir = ( int ) light->getDirection();

		SDL_Surface *surf = this->resources.lightTextures[ color ][ dir ];
		SDL_BlitScaled( surf, NULL, this->screen, &rect );
	}

	std::vector< Car* > *cars = state->getCars();

	for( int i = 0; i < cars->size(); i++ )
	{
		Car *car = ( *cars )[ i ];

		SDL_Surface *surf;
		enum Car::DIRECTIONS dir = car->getDirection();

		int carTex = car->whichTexture();

		switch( dir )
		{
			case Car::DIRECTIONS::UP:
				surf = this->resources.carTextures[ carTex ][ 0 ];
				break;

			case Car::DIRECTIONS::DOWN:
				surf = this->resources.carTextures[ carTex ][ 1 ];
				break;

			case Car::DIRECTIONS::RIGHT:
				surf = this->resources.carTextures[ carTex ][ 2 ];
				break;

			case Car::DIRECTIONS::LEFT:
				surf = this->resources.carTextures[ carTex ][ 3 ];
				break;

			default:
				surf = this->resources.carTextures[ carTex ][ 0 ];
				break;
		}

		const int s = TileMap::TILE_SIZE;

		int w = car->spriteWidth(), h = car->spriteHeight();

		auto pos = car->getPosition();
		int x = ( int ) std::get< 0 >( pos ), y = ( int ) std::get< 1 >( pos );
		SDL_Rect rect = {	( int ) ( scale * ( x - w / 2 - cx ) + this->width / 2 ),
							( int ) ( scale * ( y - h / 2 - cy ) + this-> height / 2 ),
							( int ) ( scale * w ),
							( int ) ( scale * h ) };

		SDL_BlitScaled( surf, NULL, this->screen, &rect );
	}

	SDL_UpdateWindowSurface( this->window );
}
