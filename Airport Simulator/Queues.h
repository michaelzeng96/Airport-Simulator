#ifndef QUEUES_H
#define QUEUES_H

#include <iostream>
#include "Plane.h"
#include "Random.h"
#include <queue>
using namespace std;

extern Random my_random; // shared my_random variable with Random.h file

class Queues
{
public:
	Queues() : total_wait(0), num_served(0) {}
	int total_wait; //total wait
	int num_served; //number of planes served
	queue<Plane*> queue;
	
	int get_num_served() 
	{
		return num_served;
	}

	int get_total_wait()
	{
		return total_wait;
	} 

	virtual void update(int) = 0; //to be overridden
};

class DepartureQueue : public Queues
{
private:
	int departure_time;

public:
	void set_departure_time(int departure_time) //set departure time of queue
	{
		this->departure_time = departure_time;
	}

	void update(int clock) //function to update 
	{
		// are there planes waiting in the departure queue?
		if (!queue.empty()) 
		{
			//extract front plane
			Plane *plane = queue.front();
			if (plane->ready_takeoff_time == -1) 
			{ // new plane has arrived at the front. Update the ready_takeoff_time attribute of the plane
				plane->ready_takeoff_time = clock; 
			} 
			else 
			{
				// compute the time the plane has been waiting at the front
				if (clock - plane->ready_takeoff_time > departure_time) {
					// plane has waited long enough. remove plane from departure queue
					queue.pop();

					// calculate the wait time
					int wait_time = (clock - plane->arrival_time);

					// update total_wait and num_served
					total_wait += wait_time;
					num_served++;

					// take off!   goodbye plane
					delete plane;
				}
			}
		}
	}

	friend class ServiceQueue; //allow ServiceQueue class to access DepartureQueue private members
};

class LandingQueue : public Queues
{
private:
	double arrival_rate;   // plane arrival rate per minute

public:
	void set_arrival_rate(double arrival_rate)
	{
		this->arrival_rate = arrival_rate;
	}

	void update(int clock) 
	{
		if (my_random.next_double() < arrival_rate)
		{
			queue.push(new Plane(clock));
		}

	}

	friend class ServiceQueue;
};



#endif