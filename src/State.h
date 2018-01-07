/*
	Stany aplikacji
*/

#ifndef _STATE_H_
#define _STATE_H_

#include <SDL.h>

class Renderer;

class State
{
	public:
		State(){};
		
		virtual void init(){};
		virtual void update(){};
		virtual void render( Renderer *renderer ){};
		
		virtual bool handleEvent( SDL_Event *event ){};
};

#endif // _STATE_H_