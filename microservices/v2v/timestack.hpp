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
#define MINMSG 8 //nr
#define DISTANCEFROMLEADER 100 //cm
#define TOMS 1000 //to ms
#define INTERNALCHANNEL 170 //od4
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

		// Gaurantees that it will only return when is appropriate.
		while (this->empty()) {
			usleep(100 * TOMS);
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
		std::cout << "Follower Queue {" << this->readyQueue->size() << "}" << std::endl;
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

	while (hasListener) {

		if (getInstance()->empty()) {
			hasListener = false;
			return nullptr;
		}

		LeaderStatus leaderStatus = getInstance()->pop();

		// Execute the leader status popped.
		
		//int time = leaderStatus.distanceTraveled() * 5 * 6;

		printf("Executing (speed: %f,angle: %f, for %dms)\n", leaderStatus.speed(), leaderStatus.steeringAngle(), time);

		{
			Turn turn;
			turn.steeringAngle(leaderStatus.steeringAngle());

			Move move;
			move.percent(leaderStatus.speed());

			od4.send(turn);
			od4.send(move);
		}

		usleep(125 * TOMS);

		{
			Turn turn;
			turn.steeringAngle(0.0);

			Move move;
			move.percent(0.0);

			od4.send(turn);
			od4.send(move);
		}
	}
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

		// push a leaderstatus with an distance offset from the leader.
      if (firstTime) {
      	LeaderStatus firstTimer;
      	firstTimer.speed(0.1);
      	firstTimer.distanceTraveled(DISTANCEFROMLEADER);
      	firstTimer.steeringAngle(0);
      	getInstance()->push(firstTimer);
      }

  	}
}
