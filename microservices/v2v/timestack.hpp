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

#define DISTANCEFROMLEADER 100  // cm
#define TOMS 1000               // to ms
#define INTERNALCHANNEL 170     // od4

float ANGLEMULTIPLIER = 0.5;
float MAXFOLLOWERSPEED = 0.16;   // -1 to 1

bool hasListener = false;  // Indicates if there is already a listening thread.
bool disableDistance = true;  // Makes the timestack function for vehicles
                              // without DistanceTraveled implemented.
pthread_t listener;
TimeStack *p_inst;  // Pointer instance.
float prev_speed = 0;
uint32_t ms_from_last_msg = 0;
float steering = 0;
int MINMSG = 12;
int DELAY = 0;
int max_angle = 5;

cluon::OD4Session od4{INTERNALCHANNEL};

// These two methods are used externally, and are thus defined here.
inline void push(LeaderStatus ls);
TimeStack *getInstance();
void *loopListener(void *);
inline void addListener();

float setMINMSG(int nr) {
  MINMSG = nr;
}

float setDELAY(int ms) {
  DELAY = ms;
}

float setANGLEMUL(int ang) {
  float m = ang;
  ANGLEMULTIPLIER = m/100;
}

float setSPEED(int spd) {
  float m = spd;
  MAXFOLLOWERSPEED = m/100;
}

/**
  Wrapper around a std::queue<LeaderStatus> which uses a time-release when the
  next element is popped.

  The 'stack' part doesn't really fit with the current implemntation of using a Queue.
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
  bool ready() {
    int size = this->readyQueue->size();
    // If we have no items, there is no reason to check other cases.
    if (size == 0) {
      return true;
    }

    /* If we are not using distance, use number of queued messages as the choice
       if we determine the execution to be 'empty' or ready. */
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
    while (this->ready()) {
      usleep(100 * TOMS);
    }
    // Retrieve the item.
    LeaderStatus nextLeaderStatus = this->readyQueue->front();
    this->readyQueue->pop();

    // Deduct the distance to travel from the distance to the leader.
    this->distanceToTravelUntilCollision -= nextLeaderStatus.distanceTraveled();
    return nextLeaderStatus;
  };

  /**
    Pushes a LeaderStatus to the queue if it is not identical to the last
    message received.
      **/
  inline void push(LeaderStatus ls) {

    /* Disallow negative values for following.
       This can break the 3d-printing axis on the vehicle, and
       is thus disabled for safety. */
    if (ls.speed() < -0.03) {
      return;
    }

    // Add the distance to the car so we know it is further away.
    this->distanceToTravelUntilCollision += ls.distanceTraveled();
    // The speed is not delayed and thus sent directly when received.
    Move move;
    float spd = MAXFOLLOWERSPEED;
    if (ls.speed() <= MAXFOLLOWERSPEED) {
      spd = ls.speed();
    }

    move.percent(spd);
    od4.send(move);

    this->readyQueue->push(ls);
    steering = ls.steeringAngle(); 

    if (!getInstance()->ready()) {
      addListener();
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
Blocks while there is  messages to extrapolate. Once there are no messages
currently requiring attention. it will terminate.
**/
void *loopListener(void *) {

  // Initial delay between sterring commands of the leader vehicle.
  Turn turn;
  // Makes external cancellation possible, without knowing the thread reference.
  while (hasListener) {
    // If we have no more messages to execute, we have no purpose to follow.
    if (getInstance()->ready()) {
      hasListener = false;
      return nullptr;
    }
    LeaderStatus leaderStatus = getInstance()->pop();
    float rad = leaderStatus.steeringAngle();

    // Because our stupid V2V leader is sending Radians.
    // And because our car turns at a different steeringAngle than the leader.
    float f_angle = rad*ANGLEMULTIPLIER;

    turn.steeringAngle(f_angle);
    od4.send(turn);    
    usleep(125 * TOMS);
  }
}

/**
Adds a listener if one does not already exist, This starts up a listener loop
which decides when the next message is suppose to be interpreted.
**/
inline void addListener() {
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
