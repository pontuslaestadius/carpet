#ifndef COMMANDER_SERVICE_H
#define COMMANDER_SERVICE_H

#include <iomanip>
#include <unistd.h>
#include <sys/time.h>
#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"
#include "Messages.hpp"
#include <iostream>


static const int CID = 211;
static const int MOVE_FORWARD = 1020;
static const int TURN_DIRECTION = 1021;


class commander{
public:
	commander();
	
	void testMove();

private:
	
	std::shared_ptr<cluon::OD4Session> receivedMessage;
	Turn msgSteering;
	Move msgPedal;
	const int delay = 900;
	static uint32_t getTime();

	

};

#endif //COMMANDER_SERVICE_H
