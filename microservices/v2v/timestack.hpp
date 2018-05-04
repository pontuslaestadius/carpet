/*
Author: Pontus Laestadius

Handles all LeaderStatus messages received and interprets them correctly
according to distance, time, speed and angle.

Sources: 
http://www.cplusplus.com/reference/queue/queue/

*/

#include <unistd.h>
#include <queue>
#include <pthread.h>
#include <math.h>

#include "Messages.hpp"
#include "V2VService.hpp"

using std::queue;

class TimeStack;

#define AFTER 0                 // ms
#define MINMSG 12                // nr
#define DISTANCEFROMLEADER 100  // cm
#define DELAY 100              // ms
#define TOMS 1000               // to ms
#define INTERNALCHANNEL 170     // od4
#define MAXFOLLOWERSPEED 0.16   // -1 to 1

bool hasListener = false;  // Indicates if there is already a listening thread.
bool disableDistance = true;  // Makes the timestack function for vehicles
                              // without DistanceTraveled implemented.
pthread_t listener;
TimeStack *p_inst;  // Pointer instance.
float prev_speed = 0;
uint32_t ms_from_last_msg = 0;
float steering = 0;

cluon::OD4Session od4{INTERNALCHANNEL};

// These two methods are used externally, and are thus defined here.
inline void push(LeaderStatus ls);
TimeStack *getInstance();
void *loopListener(void *);
inline void addTimeStackListener();
bool basicallyZero(float val);
void pushOffsetFromLeader(float distance);
void artificalDelay(uint32_t timestamp);

/**
  Wrapper around a std::queue<LeaderStatus> which uses a time-release when the
  next element is popped.
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
    Determines if there are enough gap between the follower and enough
    messages to perform the next action safely.
      **/
  bool empty() {
    int size = this->readyQueue->size();
    // If we have no items, there is no reason to check other cases.
    if (size == 0) {
      return true;
    }
    float speed = this->readyQueue->front().speed();
    // If we are slowing down, we allow the following message.
    float tmp_speed = prev_speed;
    prev_speed = speed;
    if (speed < tmp_speed) {
      return false;
    }

    // If we are not using distance, use number of queued messages as the choice
    // if we determine the execution to be 'empty' or ready.
    if (disableDistance) {
      return size < MINMSG;
    } else {
      return size < MINMSG &&
             // Makes sure that if we pop an item, we are not closer than
             // DISTANCEFROMLEADER to the leader vehicle.
             (this->distanceToTravelUntilCollision -
              this->readyQueue->front().distanceTraveled()) <
                 DISTANCEFROMLEADER;
    }
  }

  /**
    Blocks until it determines it's the correct time to execute the next
    message, it garantuees to always return a LeaderStatus or wait forever.
      **/
  LeaderStatus pop() {
    // Gaurantees that it will only return when is appropriate.
    while (this->empty()) {
      usleep(100 * TOMS);
    }
    // Retrieve the item.
    LeaderStatus nextLeaderStatus = this->readyQueue->front();
    this->readyQueue->pop();
    // Adds a delay if we are using delay based readings.
    artificalDelay(nextLeaderStatus.timestamp());
    // Deduct the distance to travel from the distance to the leader.
    this->distanceToTravelUntilCollision -= nextLeaderStatus.distanceTraveled();
    return nextLeaderStatus;
  };

  /**
    Pushes a LeaderStatus to the queue if it is not identical to the last
    message received.
      **/
  inline void push(LeaderStatus ls) {
    // Ignore duplicate messages being received.
    if (this->readyQueue->size() > 0) {
      LeaderStatus front = this->readyQueue->front();

      // If 'identifical' to last message.
      /*
      if (basicallyZero(front.speed()) == basicallyZero(ls.speed()) &&
          basicallyZero(front.steeringAngle()) ==
              basicallyZero(ls.steeringAngle()) &&
          basicallyZero(front.distanceTraveled()) ==
              basicallyZero(ls.distanceTraveled())) {
        return;
      }
      */
    }
    // Add the distance to the car so we know it is further away.
    this->distanceToTravelUntilCollision += ls.distanceTraveled();
    // The speed is not delayed and thus sent directly when received.
    Move move;
    float spd = MAXFOLLOWERSPEED;
    if (ls.speed() <= MAXFOLLOWERSPEED) {
      spd = ls.speed();
    }

    if (ls.speed() < -0.03) {
      return;
    }

    move.percent(spd);
    od4.send(move);

    if (!basicallyZero(ls.steeringAngle() - steering)) {
        this->readyQueue->push(ls);
        steering = ls.steeringAngle(); 

        if (!getInstance()->empty()) {
          addTimeStackListener();
        }
    }

  };
};

/**
Returns a global singleton instance of a TimeStack.
**/
TimeStack *getInstance() {
  if (!p_inst) {
    p_inst = new TimeStack();
  }
  return p_inst;
}

/**
Blocks while there is exists messages to extrapolate. Once there are no messages
currently requiring attention. it will terminate.
**/
void *loopListener(void *) {

  usleep((DELAY -DELAY*prev_speed*0.2) * TOMS);

  // Initial delay between sterring commands of the leader vehicle.
  Turn turn;
  // Makes external cancellation possible, without knowing the thread reference.
  while (hasListener) {
    // If we have no more messages to execute, we have no purpose to follow.
    if (getInstance()->empty()) {
      hasListener = false;
      return nullptr;
    }
    LeaderStatus leaderStatus = getInstance()->pop();
    // Converts the leader status to internal messages.

    // Converts rad to degress and sets a max.
    float rad = leaderStatus.steeringAngle();

    // Because our stupid V2V leader is sending Radians instead of SteeringAngle....
    float f_angle = rad * 180 / M_PI;

    float max_angle = 10;
    if (f_angle > max_angle) {
      f_angle = max_angle;
    } else if (f_angle < -max_angle) {
      f_angle = -max_angle;
    }

    turn.steeringAngle(f_angle);

    od4.send(turn);    
    usleep(125 * TOMS);
  }
}

/**
Adds a listener if one does not already exist, This starts up a listener loop
which decides when the next message is suppose to be interpreted.
**/
inline void addTimeStackListener() {
  // Make sure there can only be one.
  if (false == hasListener) {
    hasListener = true;
    // Spawn a loop thread for popping leader status messages.
    int result = pthread_create(&listener, NULL, loopListener, NULL);
    if (result != 0) {
      std::cerr << "Can't create pthread-follower-listener. Error code: "
                << result << std::endl;
    }
  }
}

/**
Values close to zero are evalulated as zero.
**/
bool basicallyZero(float val) {
  float zeroish = 0.03;
  return val <= zeroish && val >= -zeroish;
}

/**
Sleeps for a set amount of time after the given timestamp.
**/
void artificalDelay(uint32_t timestamp) {
  // If we are adding an artifical delay.
  if (AFTER > 0) {
    // This is a copycat if the way the initial timestamps are generated to match.
    timeval now;
    gettimeofday(&now, nullptr);
    uint32_t start = (uint32_t)now.tv_usec / 1000;
    // Release the element AFTER a certain time from the initial received time.
    uint32_t waitUntil = timestamp + AFTER;
    // Sleep until it is time to excecute.
    int wait = (waitUntil - start);
    if (wait > 0) {
      usleep(wait * TOMS);
    }
  }
}
