#include "command.h"

command command::command_change_light(Light::DIRECTIONS dir)
{
	return command {COMMAND_TYPE::CHANGE_LIGHT, dir, 0};
}

command command::command_wait(Light::DIRECTIONS dir, int wait_time)
{
	return command {COMMAND_TYPE::WAIT, dir, wait_time};
}
