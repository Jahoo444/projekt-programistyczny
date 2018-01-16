#include "Light.h"
#include "command.h"

Light::Light( enum DIRECTIONS dir, enum COLORS color, Uint32 startTime ) : commands{}
{
	this->dir = dir;
	this->color = color;
	this->startTime = startTime;
	this->switchTime = startTime;
}

void Light::addCommand(command& c)
{
	commands.push(c);
}

command Light::getCommand()
{
	if(commands.size() == 0)
	{
		commands.push(command::command_wait(this->dir, 8000));
		commands.push(command::command_change_light(this->dir));
	}

	if(commands.size() > 0)
		return commands.front();
	else
		return command::command_wait(this->dir, 500);
}

void Light::popFirstCommand()
{
	if(commands.size() > 0)
		commands.pop();
}

void Light::clearCommands()
{
	std::queue<command> empty;
   	std::swap( commands, empty );
}
