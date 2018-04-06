/*
  Author: Sebastian Fransson
  Created: 4/4 - 2018
  Last Updated: 6/4 - 2018

  Inspired by V2VProtocol: https://github.com/DIT168-V2V-responsibles/v2v-protocol
*/
#include "commander.hpp"

int main(){

	std::shared_ptr<commander> commanderService = std::make_shared<commander>();
	const int delay = 900;
	std::cout << "Starting service.." << std::endl;
	//Enter the chosen set of movements. OBS: This is for testing.
	while(1){
	 int choice;
	 std::this_thread::sleep_for(std::chrono::milliseconds(delay));
       	 std::cout << "(1) testMove" << std::endl;
         std::cout << "(2) testTurnLeft" << std::endl;
         std::cout << "(3) testTurnRight" << std::endl;
	 std::cout << "(4) testStop" << std::endl;
	 std::cin >> choice;
	 switch(choice){
	   case 1:{ commanderService->testMove(); break;
	   }
	   case 2:{ commanderService->testTurnLeft(); break;
	   }
	   case 3:{ commanderService->testTurnRight(); break;
	   }
	   case 4:{ commanderService->testStop(); break;
	   }
	   default: ;
	}

    }
	
}

//Control car through OD4 messages received etc.
commander::commander(){	

    receivedMessage =
        std::make_shared<cluon::OD4Session>(CID,
          [this](cluon::data::Envelope &&envelope) noexcept {
		std::cout << "OD4 Session " << std::endl;
              switch (envelope.dataType()) {
                  case TURN_DIRECTION: { //Remember to check at what angle it wants to turn.
                      opendlv::proxy::GroundSteeringReading trn = cluon::extractMessage<opendlv::proxy::GroundSteeringReading>(std::move(envelope));
                      std::cout << "received 'TURN' with angle  " << trn.steeringAngle() << " from controller'" << std::endl; 
		      msgSteering.steeringAngle(trn.steeringAngle()); //Turn appropriately...
		      std::cout << "'TURN' message sent to vehicle" << std::endl;
		       
                      break;
		}

		   case MOVE_FORWARD: {
			opendlv::proxy::PedalPositionReading mo = cluon::extractMessage<opendlv::proxy::PedalPositionReading>(std::move(envelope));
			std::cout << "received 'MOVE' from controller with speed  " << mo.percent() << std::endl;
			msgPedal.percent(mo.percent()); //Move forward
		      break;
                  }

                  default: std::cout << "No matching case, wrong message type!" << std::endl;
              }
    });

}

//Testing methods.
void commander::testMove(){
	opendlv::proxy::PedalPositionReading testMove;
	testMove.percent(0.25);
	receivedMessage->send(testMove);
}

void commander::testTurnLeft(){
	opendlv::proxy::GroundSteeringReading testTurnLeft;
	testTurnLeft.steeringAngle(15.0);
	receivedMessage->send(testTurnLeft);
}

void commander::testTurnRight(){
	opendlv::proxy::GroundSteeringReading testTurnRight;
	testTurnRight.steeringAngle(-15.0);
	receivedMessage->send(testTurnRight);
}

void commander::testStop(){
	opendlv::proxy::PedalPositionReading testStop;
	testStop.percent(0.0);
	receivedMessage->send(testStop);
}

