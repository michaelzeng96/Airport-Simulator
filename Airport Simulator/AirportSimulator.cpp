/** 
	CS273 HW6 Queues and Simulation
	Complete the parts with the FIXME comments:
		2 in Simulator.h
		1 in LandingQueue.h
		7 in ServiceQueue.h
		3 in DepartureQueue.h
    @author Whitworth CS Department
	@since  10-29-2013
*/ 

#include "Simulator.h"

int main()
{
	Simulator airport; // create the simulator
	airport.enter_data();  // enter data for the simulator
	airport.run_simulation(); // run the simulation
	airport.show_stats(); // report the run statistics
}