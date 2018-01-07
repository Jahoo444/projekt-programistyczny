/*
	Klasa definiujaca na co i jak patrzymy
*/

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <tuple>

class Camera
{
	public:
		float MOVE_SPEED = 8.0f;
		
		Camera( float x, float y, float zoom, float maxx, float maxy )
		{
			this->x = x;
			this->y = y;
			this->zoom = zoom;
			this->maxx = maxx;
			this->maxy = maxy;
		}
		
		void move( float dx, float dy )
		{
			this->x += dx * this->zoom; this->y += dy * this->zoom;
			
			return;
			
			if( this->x < 0.0f )
				this->x = 0.0f;
			else if( this->x > maxx )
				this->x = maxx;
			
			if( this->y < 0.0f )
				this->y = 0.0f;
			else if( this->y > maxy )
				this->y = maxy;
		}
		
		void zoomin()
		{
			this->zoom += ZOOM_SPEED;
			
			if( this->zoom < 0.2f )
				this->zoom = 0.2f;
			else if( this-> zoom > 8.0f )
				this->zoom = 8.0f;
		}
		
		void zoomout()
		{
			this->zoom -= ZOOM_SPEED;
			
			if( this->zoom < 0.2f )
				this->zoom = 0.2f;
			else if( this-> zoom > 8.0f )
				this->zoom = 8.0f;
		}
		
		float getZoom() { return this->zoom; }
		
		std::pair< float, float > getPosition() { return std::make_pair( this->x, this->y ); }
	
	private:
		float x, y;	// polozenie centrum kamery
		float zoom;
		float maxx, maxy;
		
		float ZOOM_SPEED = 0.05f;
};

#endif // _CAMERA_H_