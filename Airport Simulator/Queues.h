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

	virtual void update(int) = 0; //pure virtual function
	int get_num_served()
	{
		return num_served;
	}

	int get_total_wait()
	{
		return total_wait;
	}

	virtual void set_arrival_rate(double arrival_rate)
	{
		//do nothing
	}

	virtual void set_departure_time(int departure_time) //set departure time of queue
	{
		//do nothing
	}
	virtual void set_service_times(int min_service_time, int max_service_time) 
	{
		//do nothing
	}
	virtual void set_landing_queue(Queues *landing_queue) 
	{
		//do nothing
	}
	virtual void updateTotalWait(int time)
	{
		//do nothing
	}
	virtual void update_num_served()
	{
		//do nothing
	}
	virtual void set_departure_queue(Queues *departure_queue) 
	{
		//do nothing
	}
	queue<Plane*> *access_queue()
	{
		return &queue;
	}

protected:
	int total_wait; //total wait
	int num_served; //number of planes served
	queue<Plane*> queue;



};

class DepartureQueue : public Queues
{
private:
	int departure_time; //plane time of departure
public:
	void updateTotalWait(int time)
	{
		total_wait += time;
	}
	void update_num_served()
	{
		num_served++;
	}

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
};

class LandingQueue : public Queues
{
private:
	double arrival_rate;   // plane arrival rate per minute
public:
	void updateTotalWait(int time)
	{
		total_wait += time;
	}
	void update_num_served()
	{
		num_served++;
	}
	void update(int clock) 
	{
		if (my_random.next_double() < arrival_rate)
		{
			queue.push(new Plane(clock));
		}
	}

	void set_arrival_rate(double arrival_rate)
	{
		this->arrival_rate = arrival_rate;
	}
};

class ServiceQueue : public Queues
{
private:
	int min_service_time, max_service_time;  // range of service times
	Queues *landing_queue;             // pointer to the landing queue
	Queues *departure_queue;         // pointer to the departure queue

public:
	void set_service_times(int min_service_time, int max_service_time) {
		this->min_service_time = min_service_time;
		this->max_service_time = max_service_time;
	}

	void set_landing_queue(Queues *landing_queue) {
		this->landing_queue = landing_queue;
	}

	void set_departure_queue(Queues *departure_queue) {
		this->departure_queue = departure_queue;
	}

	void update(int clock)
	{
		// there is a plane at the gate
		if (!queue.empty()) {

			Plane *plane = queue.front();

			// check if a plane is ready to move from the service queue to the departure queue
			if ((clock - plane->start_service_time) >= plane->service_time)
			{
				//remove plane from the service queue and add to departure queue
				queue.pop();
				departure_queue->access_queue()->push(plane);
			}
		}

		// the gate is empty - ready to serve!
		if (queue.empty()) {

			// move a plane from the landing queue to the service queue
			if (!landing_queue->access_queue()->empty()) {

				Plane *plane = landing_queue->access_queue()->front();
				landing_queue->access_queue()->pop();

				// calculate the wait time of the plane in the landing queue

				int wait = (clock - plane->arrival_time);

				// update total_wait and num_served for the landing queue

				landing_queue->updateTotalWait(wait);

				landing_queue->update_num_served();


				// update the start_service_time attribute for the plane

				plane->start_service_time = clock;

				// compute a random service time for the plane between min_service_time and max_service_time

				int time;
				do
				{
					time = my_random.next_int(max_service_time); //get a random value between 0 and max_service_time
					if (time >= min_service_time)
						break;

				} while (time < min_service_time);

				plane->service_time = time;

				// add the plane to the service queue
				queue.push(plane);

			}
		}
	}
};

#endif