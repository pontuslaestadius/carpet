/*

Author: Pontus Laestadius

*/

#include <unistd.h>
#include <queue>
#include <pthread.h>

#include "Messages.hpp"
#include "V2VService.hpp"

using std::queue;

#define AFTER 1000
bool hasListener = false;
pthread_t listener;
bool firstTime = true;

inline void push(LeaderStatus ls);


class TimeStack {
private:
	queue<LeaderStatus> *readyQueue;

public:
	TimeStack(TimeStack const&) = delete;
	void operator=(TimeStack const&) = delete;

    static TimeStack* getInstance() {
            static TimeStack* instance;
            return instance;
    }

	TimeStack() {
		readyQueue = new queue<LeaderStatus>;
	}

	~TimeStack() {
		free(this->readyQueue);
	};

	bool empty() {
		return this->readyQueue->empty();
	}

	LeaderStatus pop() {
		timeval now;
    	gettimeofday(&now, nullptr);
    	uint32_t rightNow = (uint32_t) now.tv_usec / 1000;

		uint32_t waitUntil = rightNow +AFTER;
		LeaderStatus nextLeaderStatus = this->readyQueue->front();
		this->readyQueue->pop();

		if (waitUntil < rightNow) {
			unsigned int microseconds = waitUntil - rightNow;
			usleep(microseconds);
		}
		
		return nextLeaderStatus;
	};

	inline void push(LeaderStatus ls) {
		this->readyQueue->push(ls);
	};
};

void* loopListener(void*) {

	while (hasListener) {

		if (TimeStack::getInstance()->empty()) {
			hasListener = false;
			return nullptr;
		}

		LeaderStatus ls = TimeStack::getInstance()->pop();
		// Do the execution stuff.
	}

}

inline void addTimeStackListener() {
	if (false == hasListener) {
      int result = pthread_create(&listener, NULL, loopListener, NULL);

      if (result == 0) {
        std::cout << "Enable to create listener thread" << std::endl;
        return;
      }

      if (firstTime) {
      	// TODO push a leaderstatus with 1m offset.
      }

      hasListener = false;
  	}
}

