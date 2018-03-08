#include <cstdint>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"

#include "messages.hpp"

double angle = 0.0;
double speed = 0.0;
const int delay = 1500;

int main(int /*argc*/, char** /*argv*/) {

    cluon::OD4Session od4(111,[](cluon::data::Envelope &&envelope) noexcept {
        if (envelope.dataType() == opendlv::proxy::GroundSteeringReading::ID()) {
            opendlv::proxy::GroundSteeringReading receivedMsg = cluon::extractMessage<opendlv::proxy::GroundSteeringReading>(std::move(envelope));
            std::cout << "Sent a message for ground steering to " << receivedMsg.steeringAngle() << "." << std::endl;
        }
        else if (envelope.dataType() == opendlv::proxy::PedalPositionReading::ID()) {
            opendlv::proxy::PedalPositionReading receivedMsg = cluon::extractMessage<opendlv::proxy::PedalPositionReading>(std::move(envelope));
            std::cout << "Sent a message for pedal position to " << receivedMsg.percent() << "." << std::endl;
        }
    });

    if(od4.isRunning() == 0)
    {
        std::cout << "ERROR: No od4 running!!!" << std::endl;
        return -1;
    }

    opendlv::proxy::GroundSteeringReading msgSteering;
    opendlv::proxy::PedalPositionReading msgPedal;

	int len = 9;
	char foo [len] = { 'r', 'w', 'a', 'd', 'w', 'd', 'a', 'd', 'r'};

	for (int i = 0; i < len; i++) {
		std::this_thread::sleep_for(std::chrono::milliseconds(delay));
		switch (foo[i]) {
		case 'w':
			   msgPedal.percent(0.25);
			   od4.send(msgPedal);
		break;	
		case 's':
			   msgPedal.percent(-0.1);
			   od4.send(msgPedal);
		break;
		case 'a':
			   msgSteering.steeringAngle(-15.0);
    		   	   od4.send(msgSteering);
		break;
		case 'd':
			   msgSteering.steeringAngle(15.0);
    		   	   od4.send(msgSteering);
		break;
		case 'r':
			   msgSteering.steeringAngle(0.0);
    		   	   od4.send(msgSteering);
			   msgPedal.percent(0.0);
			   od4.send(msgPedal);
		break;
		default:
		        std::cout << "Invalid command character: " << foo[i] << std::endl;
		continue;
		}
	}

    return 0;
}

