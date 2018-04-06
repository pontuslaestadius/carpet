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

};

#endif //COMMANDER_SERVICE_H
