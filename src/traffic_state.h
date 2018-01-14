#ifndef TRAFFIC_STATE_H
#define TRAFFIC_STATE_H
#include "Light.h"
#include <vector>
#include <tuple>
#include <iostream>
#include <ostream>
#include <iomanip>
using intersection_node = std::tuple<int, double, Light::COLORS>;

constexpr int padding = 15;

struct traffic_state {
    long unsigned int cars_number;
    double avg_wait_time;
    std::vector<intersection_node> intensity;
    friend std::ostream& operator<<(std::ostream& os, const traffic_state& obj)
    {
        os<<std::setw(padding)<<std::left<<"Cars:"<<std::setw(padding)<<std::left<<obj.cars_number<<std::endl;
        os<<std::setw(padding)<<std::left<<"Time on crossroad:"<<std::setw(padding)<<std::left<<std::setprecision(3)<<obj.avg_wait_time<<std::endl;
        os<<"Traffic per segment:"<<std::endl;
        for(const auto& it : obj.intensity)
        {
            os<<std::setw(padding)<<std::left<<"Cars:"<<std::setw(padding)<<std::left<<std::get<0>(it)<<std::endl;
            os<<std::setw(padding)<<std::left<<"Time on crossroad:"<<std::setw(padding)<<std::left<<std::setprecision(3)<<std::get<1>(it)<<std::endl;
            os<<std::setw(padding)<<std::left<<"Light color:"<<std::setw(padding)<<std::left<<std::get<2>(it)<<std::endl;
            os<<std::endl;
        }
        return os;
    };
};


#endif
