#ifndef COMMANDER_SERVICE_H
#define COMMANDER_SERVICE_H

#include <iomanip>
#include <unistd.h>
#include <sys/time.h>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "messages.hpp"
#include <iostream>


static const int CID = 111;
static const int TURN_DIRECTION = 1045;
static const int MOVE_FORWARD = 1041;
//V2V id's --------
static const int ANNOUNCE_PRECENSE = 1001;
//Follower
static const int FOLLOW_REQUEST = 1002;
static const int FOLLOW_RESPONSE = 1003;
static const int STOP_FOLLOW = 1004;
static const int FOLLOWER_STATUS = 3001;
//Leader
static const int LEADER_STATUS = 2001;


class commander{
public:
	commander();
	
	void testMove();
	void testTurnLeft();
	void testTurnRight();
	void testStop();

private:
	
	std::shared_ptr<cluon::OD4Session> receivedMessage;
	opendlv::proxy::GroundSteeringReading msgSteering;
	opendlv::proxy::PedalPositionReading msgPedal;

	//Test variables for V2V ----
	AnnouncePrecense apTest;
	FollowRequest frqTest
	FollowResponse flrTest;
	StopFollow stpTest;
	FollowerStatus flstatTest;
	LeaderStatus ldrstatTest;

};

#endif //COMMANDER_SERVICE_H
