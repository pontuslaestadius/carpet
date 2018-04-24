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

#define AFTER 1000 //ms
#define MINMSG 5 //nr
#define DISTANCEFROMLEADER 100 //cm
#define TOMS 1000 
bool hasListener = false;	// Indicates if there is already a listening thread.
bool firstTime = true;		// Used to set the initial distance from the leader vehicle.
pthread_t listener;
TimeStack *p_inst;

inline void push(LeaderStatus ls);
void executeOrder66(LeaderStatus leaderStatus);
TimeStack* getInstance();

/**

Wrapper around a std::queue<LeaderStatus> which uses a time-release
when the next element is popped.

**/
class TimeStack {
private:
	queue<LeaderStatus> *readyQueue = new queue<LeaderStatus>;

public:
	TimeStack() {}
	~TimeStack() {
		free(this->readyQueue);
		free(this);
	};

	/**
	
	Determines if there are enough messages to currently follow.

	**/
	bool empty() {
		return this->readyQueue->size() < MINMSG;
	}

	/**

	Blocks until it determines it's the correct time to 
	execute the next message, it garantuees to always
	return a LeaderStatus or wait forever.

	**/
	LeaderStatus pop() {

		while (this->empty()) {
			printf("Pop sleep\n");
			usleep(100 * TOMS);
			printf("Pop wake\n");
		}

		timeval now;
    	gettimeofday(&now, nullptr);
    	uint32_t start = (uint32_t) now.tv_usec / 1000;

		LeaderStatus nextLeaderStatus = this->readyQueue->front();
		this->readyQueue->pop();

		// Release the element AFTER a certain time from the initial received time.
		uint32_t waitUntil = nextLeaderStatus.timestamp() +AFTER;

		if (start < waitUntil) {
			printf("Pop wait, %d\n", (waitUntil - start));
			usleep((waitUntil - start) * TOMS);
		}
		
		return nextLeaderStatus;
	};

	inline void push(LeaderStatus ls) {
		printf("pushed leaderstatus to queue %d\n", this->readyQueue->size());

		this->readyQueue->push(ls); // <--------------------------------------------- Causes segmentation fault.

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
	std::cout << "loopListener" << std::endl;

	while (hasListener) {
		std::cout << "loopListener new loop" << std::endl;

		if (getInstance()->empty()) {
			std::cout << "loopListener ended, no messages" << std::endl;
			hasListener = false;
			return nullptr;
		}

        std::cout << "Popping" << std::endl;

		LeaderStatus ls = getInstance()->pop();
		executeOrder66(ls);
	}
		std::cout << "loopListener ended" << std::endl;
}

/**

Adds a listener if one does not already exist,
This starts up a listener loop which decides 
when the next message is suppose to be interpreted.

**/
inline void addTimeStackListener() {

	if (false == hasListener) {
		hasListener = true;

      int result = pthread_create(&listener, NULL, loopListener, NULL);

      if (result != 0) {
        std::cerr << "Can't create phread for Follower Listener" << std::endl;
        return;
      }

      if (firstTime) {
      	std::cout << "and it's the first time. ";
      	// push a leaderstatus with 1m offset.
      	LeaderStatus firstTimer;
      	firstTimer.speed(0.1);
      	firstTimer.distanceTraveled(DISTANCEFROMLEADER);
      	firstTimer.steeringAngle(0);

      	std::cout << "So I made a LeaderStatus, ";

      	getInstance()->push(firstTimer);

      	std::cout << "and added it to the global instance. " << std::endl;
      }

  	}
}

// Follower #2 executing the commands.
void executeOrder66(LeaderStatus leaderStatus) {

	printf("Executing (%f, %f, %f)\n", leaderStatus.speed(), leaderStatus.steeringAngle(), leaderStatus.distanceTraveled());
}