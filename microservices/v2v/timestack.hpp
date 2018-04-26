/*

Author: Pontus Laestadius

*/

#include <unistd.h>
#include <queue>
#include <pthread.h>

#include "Messages.hpp"
#include "V2VService.hpp"

using std::queue;

class TimeStack;

#define AFTER 2100 //ms
#define MINMSG 8 //nr
#define DISTANCEFROMLEADER 100 //cm
#define TOMS 1000 //to ms
#define INTERNALCHANNEL 170 //od4

bool hasListener = false;	// Indicates if there is already a listening thread.
bool firstTime = true;		// Used to set the initial distance from the leader vehicle.
pthread_t listener;
TimeStack *p_inst;

inline void push(LeaderStatus ls);
TimeStack* getInstance();

/**

Wrapper around a std::queue<LeaderStatus> which uses a time-release
when the next element is popped.

**/
class TimeStack {
private:
	queue<LeaderStatus> *readyQueue = new queue<LeaderStatus>;
	float distanceToTravelUntilCollision = 0;

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
		return this->readyQueue->size() > MINMSG && 
		(this->distanceToTravelUntilCollision - this->readyQueue->front().distanceTraveled()) < DISTANCEFROMLEADER;
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

		// Generate a timestamp, to compare with the leaderstatus.
		timeval now;
    	gettimeofday(&now, nullptr);
    	uint32_t start = (uint32_t) now.tv_usec / 1000;

    	// Retrieve the item to pop.
		LeaderStatus nextLeaderStatus = this->readyQueue->front();
		this->readyQueue->pop();

		// Release the element AFTER a certain time from the initial received time.
		uint32_t waitUntil = nextLeaderStatus.timestamp() +AFTER;

		// Sleep until it is time to excecute.
			std::cout << "sleep for: " << (waitUntil - start) << " ms" << std::endl;
			usleep((waitUntil - start) * TOMS);
		}
		
		// Deduct the distance to travel from the distance to the leader.
		this->distanceToTravelUntilCollision -= nextLeaderStatus.distanceTraveled();
		return nextLeaderStatus;
	};

	inline void push(LeaderStatus ls) {

		// Ignore duplicate messages being received.
		if (this->readyQueue->size() > 0) {
			LeaderStatus front = this->readyQueue->front();

			// If identifical to last message.
			if (
				front.speed() == ls.speed() &&
				front.steeringAngle() == ls.steeringAngle() &&
				front.distanceTraveled() == ls.distanceTraveled()
				) {
				return;
			}
			
		}

		// Add the distance to the car so we know it is further away.
		this->distanceToTravelUntilCollision += ls.distanceTraveled();
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

    // Makes external cancellation possible, without knowing the thread reference.
	while (hasListener) {

		// If we have no more messages to execute, we have no purpose to follow.
		if (getInstance()->empty()) {
			hasListener = false;
			return nullptr;
		}


		// Pop a leader status.
		LeaderStatus leaderStatus = getInstance()->pop();

		// Converts the leader status to internal messages and send them to the commander.
		Turn turn;
		Move move;

		turn.steeringAngle(leaderStatus.steeringAngle());
		move.percent(leaderStatus.speed());

		std::cout << "[TIMESTACK]: " << leaderStatus.speed() << " " << leaderStatus.steeringAngle() << " " <<  leaderStatus.distanceTraveled() << std::endl;

		od4.send(turn);
		od4.send(move);
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
        return;
      }

		// push a leaderstatus with an distance offset from the leader.
      if (firstTime) {
      	/*
      		LeaderStatus firstTimer;
	      	firstTimer.speed(0.14);
	      	firstTimer.distanceTraveled(DISTANCEFROMLEADER);
	      	firstTimer.steeringAngle(0);
	      	getInstance()->push(firstTimer);
	      	*/
      }

  	}
}
