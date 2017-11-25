#ifndef DEPARTURE_Q_H_
#define DEPARTURE_Q_H_

#include <queue>
#include "Plane.h"

class DepartureQueue
{
private:
	std::queue<Plane *> the_queue;  // the queue of planes in the departure queue
	int departure_time;  // the time a plane has to wait in the queue before it can take-off
	int total_wait;   // total accumulated wait time in the departure queue
	int num_served;   // number of planes served through the departure queue
public:
	DepartureQueue() : total_wait(0), num_served(0) {}

	void set_departure_time(int departure_time) {
		this->departure_time = departure_time;
	}

	int get_num_served() {
		return num_served;
	}

	int get_total_wait() {
		return total_wait;
	}

	void update(int clock) 
	{
		// are there planes waiting in the departure queue?
		if (!the_queue.empty()) 
		{
			//extract front plane
			Plane *plane = the_queue.front();
			if (plane->ready_takeoff_time == -1) // if new plane has arrived at the front
			{ 
				plane->ready_takeoff_time = clock; // update the ready_takeoff_time attribute of the plane
			} 
			else // compute the time the plane has been waiting at the front
			{
				
				if (clock - plane->ready_takeoff_time > departure_time) //check if plane has waited long enough in queue
				{
					// remove plane from departure queue
					the_queue.pop();

					// calculate the wait time for that plane
					int wait_time = (clock - plane->arrival_time);

					// update total_wait and num_served
					total_wait += wait_time;
					num_served++;

					// plane takes off, delete from departure queue
					delete plane;
				}
			}
		}
	
	}

	friend class ServiceQueue;
	
};

#endif
