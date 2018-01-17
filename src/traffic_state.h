#ifndef TRAFFIC_STATE_H
#define TRAFFIC_STATE_H
#include "Light.h"

#include <vector>
#include <tuple>
#include <iostream>
#include <ostream>



using intersection_node = std::tuple<Light::DIRECTIONS, int, double, Light::COLORS, int>;

struct traffic_state {
    long unsigned int cars_number;
    double avg_wait_time;
	double max_wait_time;
    std::vector<intersection_node> intensity;
    friend std::ostream& operator<<(std::ostream& os, const traffic_state& obj);
};


#endif
