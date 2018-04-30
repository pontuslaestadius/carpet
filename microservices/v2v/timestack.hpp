/*

Author: Pontus Laestadius

Purpose:

Handles all LeaderStatus messages received and interprets them correctly according to distance, time, speed and angle.

*/

#include <unistd.h>
#include <queue>
#include <pthread.h>

#include "Messages.hpp"
#include "V2VService.hpp"

using std::queue;

class TimeStack;

#define AFTER 000 //ms
#define MINMSG 8 //nr
#define DISTANCEFROMLEADER 100 //cm
#define TOMS 1000 //to ms
#define INTERNALCHANNEL 170 //od4

bool hasListener = false;	// Indicates if there is already a listening thread.
bool firstTime = true;		// Used to set the initial distance from the leader vehicle.
bool debug = true;
bool disableDistance = true; // Makes the timestack function for vehicles without DistanceTraveled implemented.
pthread_t listener;
TimeStack *p_inst; // Pointer instance.

// These two methods are used externally, and are thus defined here.
inline void push(LeaderStatus ls);
TimeStack* getInstance();
inline void addTimeStackListener();
bool basicallyZero(float val);

/**

Unifies all output to the cout by prepending a notation.

**/
bool debug_mode() {
	if (debug) {
		std::cout << "[TIMESTACK]: ";
	}
	return debug;
}

/**

Wrapper around a std::queue<LeaderStatus> which uses a time-release
when the next element is popped.

**/
class TimeStack {
private:
	queue<LeaderStatus> *readyQueue = new queue<LeaderStatus>;
	float distanceToTravelUntilCollision = DISTANCEFROMLEADER;

public:
	TimeStack() {}
	~TimeStack() {
		free(this->readyQueue);
		free(this);
	};

	/**
	
	Determines if there are enough gap between the follower and 
	enough messages to perform the next action safely.

	**/
	bool empty() {
		int size = this->readyQueue->size();

		if (size == 0) {
			return true;
		}

		int speed = this->readyQueue->front().speed();
		if (basicallyZero(speed)) {
			return false;
		}

		if (disableDistance) {
			return size < MINMSG;
		} else {
			return size < MINMSG && 
			// Makes sure that if we pop an item, we are not closer than DISTANCEFROMLEADER to the leader vehicle.
			(this->distanceToTravelUntilCollision - this->readyQueue->front().distanceTraveled()) < DISTANCEFROMLEADER;
		}

	}

	/**

	Blocks until it determines it's the correct time to 
	execute the next message, it garantuees to always
	return a LeaderStatus or wait forever.

	**/
	LeaderStatus pop() {

		// Gaurantees that it will only return when is appropriate.
		while (this->empty()) {
			usleep(100 * TOMS);
		}

		// Generate a timestamp to compare with the leaderstatus.
		timeval now;
		gettimeofday(&now, nullptr);
		uint32_t start = (uint32_t) now.tv_usec / 1000;

		// Retrieve the item.
		LeaderStatus nextLeaderStatus = this->readyQueue->front();
		this->readyQueue->pop();

		// Release the element AFTER a certain time from the initial received time.
		uint32_t waitUntil = nextLeaderStatus.timestamp() +AFTER;

		// Sleep until it is time to excecute.
		int wait = (waitUntil - start);
		if(wait > 0){
				if (debug_mode()) {
					std::cout << "Execute next message in: " << wait << " ms" << std::endl;
				}
			//usleep(wait * TOMS);
		}
		
		// Deduct the distance to travel from the distance to the leader.
		this->distanceToTravelUntilCollision -= nextLeaderStatus.distanceTraveled();

		return nextLeaderStatus;
	};

	/**

	Pushes a LeaderStatus to the queue if it is not identical to the last message received.

	**/
	inline void push(LeaderStatus ls) {

		// Ignore duplicate messages being received.
		if (this->readyQueue->size() > 0) {
			LeaderStatus front = this->readyQueue->front();

			// If identifical to last message.
			if (
				basicallyZero(front.speed()) == basicallyZero(ls.speed()) &&
				basicallyZero(front.steeringAngle()) == basicallyZero(ls.steeringAngle()) &&
				basicallyZero(front.distanceTraveled()) == basicallyZero(ls.distanceTraveled())
				) {

				return;
			}
			
		}

		// Add the distance to th e car so we know it is further away.
		this->distanceToTravelUntilCollision += ls.distanceTraveled();

		if (debug_mode() && !disableDistance) {
			std::cout << "Distance to the leader: " << this->distanceToTravelUntilCollision << std::endl;
		}

		this->readyQueue->push(ls);
	};
};

/**

Returns a global singleton instance of a TimeStack.

**/
TimeStack* getInstance() {
	if (!p_inst) {
		p_inst = new TimeStack();
	}
	return p_inst;
}

/**

Blocks while there is exists messages to extrapolate. 
Once there are no messages currently requiring attention.
it will terminate.

**/
void* loopListener(void*) {

	cluon::OD4Session od4{INTERNALCHANNEL};

	if(od4.isRunning() == 0) {
	   throw std::domain_error("no od4 running, are you connected to the interwebs?");
	}

	if (debug_mode()) {
		std::cout << "Started listening" << std::endl;
	}

	Turn turn;
	Move move;

	// Makes external cancellation possible, without knowing the thread reference.
	while (hasListener) {

		// If we have no more messages to execute, we have no purpose to follow.
		if (getInstance()->empty()) {

			if (debug_mode()) {
				std::cout << "Stopped listening" << std::endl;
			}

			hasListener = false;
			return nullptr;
		}

		LeaderStatus leaderStatus = getInstance()->pop();

		if (debug_mode()) {
			std::cout << leaderStatus.speed() << " " 
			<< leaderStatus.steeringAngle() << " " 
			<< leaderStatus.distanceTraveled() << " "
			<< leaderStatus.timestamp() << " "
			<< std::endl;
		}

		// Converts the leader status to internal messages and send them to the commander.

		turn.steeringAngle(leaderStatus.steeringAngle());
		move.percent(leaderStatus.speed());

		od4.send(turn);
		od4.send(move);
		usleep(leaderStatus.distanceTraveled() * TOMS * 10);
	}

}

/**

Adds a listener if one does not already exist,
This starts up a listener loop which decides 
when the next message is suppose to be interpreted.

**/
inline void addTimeStackListener() {

	// Make sure there can only be one.
	if (false == hasListener) {
		hasListener = true;

		// Spawn a loop thread for popping leader status messages.
		int result = pthread_create(&listener, NULL, loopListener, NULL);

		  if (result != 0) {
			std::cerr << "Can't create pthread for Follower Listener" << std::endl;
		  }

		  // We attach a starting offset meter from the vehicle in front of us.
		  if (firstTime) {
		  	pushOffsetFromLeader(DISTANCEFROMLEADER);
		  }
	}
}


void pushOffsetFromLeader(float distance) {
	firstTime = false;
  	LeaderStatus firstMeter;
  	firstMeter.speed(0.15);
  	firstMeter.distanceTraveled(distance);
  	firstMeter.steeringAngle(0);

  	getInstance()->push(firstMeter);

	LeaderStatus firstMeterStop;
  	firstMeterStop.speed(0);
  	firstMeterStop.distanceTraveled(0);
  	firstMeterStop.steeringAngle(0);

  	getInstance()->push(firstMeterStop);
}

bool basicallyZero(float val) {
	return val <= 0.03 && val >= -0.03;
}
