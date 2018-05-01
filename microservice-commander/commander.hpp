#ifndef COMMANDER_SERVICE_H
#define COMMANDER_SERVICE_H

#include <iomanip>
#include <unistd.h>
#include <sys/time.h>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "messages.hpp"
#include <iostream>

//ID's for messages and session. ---------
static const int CID = 170;
static const int forwardCID = 171;
static const int TURN_DIRECTION = 1545;
static const int MOVE_FORWARD = 1541;
static const int DISTANCE_READ = 1039;
static const int STOP = 1550;
//V2V id's --------
static const int ANNOUNCE_PRESENCE = 1001;
//Follower -----------
static const int FOLLOW_REQUEST = 1002;
static const int FOLLOW_RESPONSE = 1003;
static const int STOP_FOLLOW = 1004;
static const int FOLLOWER_STATUS = 3001;
//Leader -------------
static const int LEADER_STATUS = 2001;

// OD4 171 test variables --------------
static const int FORWARDED_MOVE = 1541;
static const int FORWARDED_TURN = 1545;

//If needed variables --------------
static const int FORWARD_MOVE = 1041;
static const int TURN_ANGLE = 1045;

//static bool follow = false;
int distRead = 0; //Keep track of the number of distance reads within range.

class commander{
//Declare the public methods and variables.
public:
	commander();
	
	// Test methods for OD4 values.
	void testMove();
	void testTurnLeft();
	void testTurnRight();
	void testStop();
	void sendFollowerStatus();

//Declare private methods and variables.
private:
	//Declares the od4 session as well as steering and pedal variables for testing and output.
	std::shared_ptr<cluon::OD4Session> receivedMessage;
	std::shared_ptr<cluon::OD4Session> forwardedMessage;
	std::shared_ptr<cluon::OD4Session> presence;

};

#endif //COMMANDER_SERVICE_H
