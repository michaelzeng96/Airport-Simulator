#ifndef PLANE_H_
#define PLANE_H_

// Object to represent a plane. It's attributes keep track of 
     
struct Plane 
{
	Plane(int clock) : 
	arrival_time(clock), start_service_time(-1), service_time(-1), ready_takeoff_time(-1) {} //init all values to -1

	int arrival_time; // when plane arrives
	int service_time; // how much time it takes to service this plane
	int start_service_time; // when the plane began to be serviced
	int ready_takeoff_time; // what time the plane enters the departure queue, and is ready to take off
};

#endif
