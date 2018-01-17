#include "traffic_state.h"
#include "enum_map.h"
#include <iomanip>

constexpr int padding = 20;

std::ostream& operator<<(std::ostream& os, const traffic_state& obj)
{
	os<<"============================="<<std::endl;
	os<<std::setw(padding)<<std::left<<"Cars:"<<std::setw(padding)<<std::left<<obj.cars_number<<std::endl;
	os<<std::setw(padding)<<std::left<<"Time on crossroad:"<<std::setw(padding)<<std::left<<std::setprecision(3)<<obj.avg_wait_time<<std::endl<<std::endl;
	os<<std::setw(padding)<<std::left<<"Max Time:"<<std::setw(padding)<<std::left<<std::setprecision(3)<<obj.max_wait_time<<std::endl<<std::endl;
	os<<"Traffic per segment:"<<std::endl;
	for(const auto& it : obj.intensity)
	{
		os<<std::setw(padding)<<std::left<<"Direction:"<<std::setw(padding)<<std::left<<enum_to_string::to_string(std::get<0>(it))<<std::endl;
		os<<std::setw(padding)<<std::left<<"Cars:"<<std::setw(padding)<<std::left<<std::get<1>(it)<<std::endl;
		os<<std::setw(padding)<<std::left<<"Time on crossroad:"<<std::setw(padding)<<std::left<<std::setprecision(3)<<std::get<2>(it)<<std::endl;
		os<<std::setw(padding)<<std::left<<"Light color:"<<std::setw(padding)<<std::left<<enum_to_string::to_string(std::get<3>(it))<<std::endl;
		os<<std::setw(padding)<<std::left<<"Command List:"<<std::setw(padding)<<std::left<<std::get<4>(it)<<std::endl;
		os<<std::endl;
	}
	os<<"============================="<<std::endl;
	return os;
}
