#include "Simulation.h"

int main( int argc, char *argv[] )
{
	Simulation simulation;
	bool precognition = true;
	if(argc == 2)
	{
		if(std::string(argv[1]).compare("-nope") == 0)
			precognition = false;
	}

	simulation.init(precognition);
	simulation.run();

	return 0;
}
