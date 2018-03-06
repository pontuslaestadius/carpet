#include <cstdint>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

#include "cluon/OD4Session.hpp"
#include "cluon/Envelope.hpp"

#include "messages.hpp"

int main(int argc, char** argv) {

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

	double angle = 0.0;
	double speed = 0.0;

	for (char **a = argv ; a != argv+argc ; a++) {
    	for(char *p = *a ; *p != '\0' ; p++) {

		    if (speed >= 0.0)
				speed -= 0.1;

		    switch (*p) {
		        case 'w':
		            speed += 0.1;
					continue;
		            break;
		        case 't':
					angle -= 10.0;
		            break;
		        case 'y':
		            angle += 10.0;
			    break;
		        default:
		        std::cout << "Invalid: " << *argv << std::endl;
		            return 0;
		    }
		    for (int i = 0; i < INT32_MAX; i++);

			if (angle > 30) angle = 30;
			if (angle < -30) angle = -30;
			if (speed > 0.25) speed = 0.25;

			std::cout << "(speed, angle) -> (" << speed << "," << angle << ")" << std::endl;

		    msgPedal.percent(speed);
		    msgSteering.steeringAngle(angle);
		    od4.send(msgPedal);
		    od4.send(msgSteering);
    	}
	}

    return 0;
}

