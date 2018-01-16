#ifndef COMMAND_H
#define COMMAND_H

#include "Light.h"

class command {
public:
	static command command_change_light(Light::DIRECTIONS dir);
	static command command_wait(Light::DIRECTIONS dir, int wait_time = 8000);
	enum COMMAND_TYPE {
		CHANGE_LIGHT,
		WAIT
	};

	COMMAND_TYPE getCommandType() { return ct; };
	Light::DIRECTIONS getLightDirection() { return dir; };
	int getTime() { return time; };
private:
	command() = delete;
	command(COMMAND_TYPE ct, Light::DIRECTIONS dir, int wait_time) : ct{ct}, dir{dir}, time{wait_time} {};
	COMMAND_TYPE ct;
	Light::DIRECTIONS dir;
	int time;
};




#endif
