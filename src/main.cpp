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
    while (*++argv) {
	if (speed >= 0.0)
		speed -= 0.1;

        switch ((*argv)[1]) {
            case 'w':
                speed += 0.1;
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
        msgPedal.percent(speed);
        msgSteering.steeringAngle(angle);
        od4.send(msgPedal);
        od4.send(msgSteering);
    }


/*
    std::cout << "Hello World again!" << std::endl;

    opendlv::proxy::GroundSteeringReading msgSteering;
    opendlv::proxy::PedalPositionReading msgPedal;

    const int delay = 1000;

    std::cout << "Now move forward ..." << std::endl;
    msgSteering.steeringAngle(0.0);
    od4.send(msgSteering);
    msgPedal.percent(0.4);
    od4.send(msgPedal);
    std::this_thread::sleep_for(std::chrono::milliseconds(2 * delay));

    std::cout << "Now turn left a little bit ..." << std::endl;
    msgPedal.percent(0.2);
    od4.send(msgPedal);
    msgSteering.steeringAngle(-15.0);
    od4.send(msgSteering);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));

    std::cout << "Now turn right a little bit ..." << std::endl;
    msgSteering.steeringAngle(15.0);
    od4.send(msgSteering);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));

    std::cout << "Now move forward again..." << std::endl;
    msgSteering.steeringAngle(0.0);
    od4.send(msgSteering);
    msgPedal.percent(0.4);
    od4.send(msgPedal);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));

    std::cout << "Now Stop ..." << std::endl;
    msgPedal.percent(0.0);
    od4.send(msgPedal);
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));

	*/
    return 0;
}

