#ifndef ENUM_TO_STRING_H
#define ENUM_TO_STRING_H

#include <map>
#include <string>

#include "Light.h"
#include "command.h"

const std::map<Light::DIRECTIONS, std::string> light_direction_map_t
    {{Light::DIRECTIONS::UP, "UP"},
    {Light::DIRECTIONS::DOWN, "DOWN"},
    {Light::DIRECTIONS::LEFT, "LEFT"},
	{Light::DIRECTIONS::RIGHT, "RIGHT"},
	{Light::DIRECTIONS::NONE, "NONE"},
	{Light::DIRECTIONS::NUM_DIRECTIONS, "NUM_DIRECTIONS"}};

const std::map<Light::COLORS, std::string> light_color_map_t
    {{Light::COLORS::GREEN, "GREEN"},
    {Light::COLORS::YELLOW, "YELLOW"},
    {Light::COLORS::RED, "RED"},
	{Light::COLORS::NUM_COLORS, "NUM_COLORS"}};


class enum_to_string {
public:
	template<typename T>
	static std::string to_string(const T& obj);
};

template<>
std::string enum_to_string::to_string(const Light::DIRECTIONS& obj)
{
	auto f = light_direction_map_t.find(obj);
	return f->second;
}

template<>
std::string enum_to_string::to_string(const Light::COLORS& obj)
{
	auto f = light_color_map_t.find(obj);
	return f->second;
}




#endif
