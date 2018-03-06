#include <cstdint>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include <unistd.h>

#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"

#include "messages.hpp"


	opendlv::proxy::GroundSteeringReading msgSteering;
	opendlv::proxy::PedalPositionReading msgPedal;

	double angle = 0.0;
	double speed = 0.0;

void action(char &p);

void safety_check();
void print_debug();
void set_msg();

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

	msgPedal.percent(0.0);
    	msgSteering.steeringAngle(0.0);
    	od4.send(msgPedal);
    	od4.send(msgSteering);
	usleep(999999999);
    
	msgPedal.percent(0.25);
    	msgSteering.steeringAngle(-15.0);
    	od4.send(msgPedal);
    	od4.send(msgSteering); 
	usleep(999999999);
	
	msgPedal.percent(-0.25);
    	msgSteering.steeringAngle(15.0);
    	od4.send(msgPedal);
    	od4.send(msgSteering); 
	usleep(999999999);

	msgPedal.percent(0.0);
    	msgSteering.steeringAngle(0.0);
    	od4.send(msgPedal);
    	od4.send(msgSteering);

    return 0;
}



