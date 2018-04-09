/*
  Author: Sebastian Fransson
  Created: 4/4 - 2018
  Last Updated: 9/4 - 2018

  Inspired by V2VProtocol: https://github.com/DIT168-V2V-responsibles/v2v-protocol
*/
#include "commander.hpp"

int main(){

	std::shared_ptr<commander> commanderService = std::make_shared<commander>();
	const int delay = 2000;
	std::cout << "Starting service.." << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	std::cout << "Service running!" << std::endl;
	
	while(1){
	std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	
	//Enter the chosen set of movements. OBS: This is for testing.
	 /*int choice;
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
	}*/
	

    }
	
}

//Control car through OD4 messages received etc.
commander::commander(){	
    //Connects to od4 session with CID:111 (Can be changed in the header file).
    receivedMessage =
        std::make_shared<cluon::OD4Session>(CID,
          [this](cluon::data::Envelope &&envelope) noexcept {
		std::cout << "OD4 Session " << std::endl;

	      //Set up response depending on the message type received through the od4 session.
              switch (envelope.dataType()) {
                   case TURN_DIRECTION: { //Remember to check at what angle it wants to turn.
			//Unpacks the envelope and extracts the contained message. (Should potentially identify steering and move commands automatically).
                        opendlv::proxy::GroundSteeringReading trn = cluon::extractMessage<opendlv::proxy::GroundSteeringReading>(std::move(envelope)); //Should be enough??
                        std::cout << "received 'TURN' with angle  " << trn.steeringAngle() << " from controller'" << std::endl; 
		        msgSteering.steeringAngle(trn.steeringAngle()); //Turn appropriately...
		        std::cout << "'TURN' message sent to vehicle" << std::endl;
		       
                        break;
		  }

		   case MOVE_FORWARD: {
			opendlv::proxy::PedalPositionReading mo = cluon::extractMessage<opendlv::proxy::PedalPositionReading>(std::move(envelope));
			std::cout << "received 'MOVE' from controller with speed  " << mo.percent() << std::endl;
			msgPedal.percent(mo.percent()); //Move forward. (Might be unnecessary, check when controller is tested).
		      	break;
                  }


		   //OBS: Below this point until the test methods is experimental for commander V2V structure....
		   //TODO: Check what needs to be handled here aswell as how requests are sent from V2V microservice..
		   case ANNOUNCE_PRECENCE: {
			AnnouncePrecence ap = cluon::extractMessage<AnnouncePrecence>(std::move(envelope));
			std::cout << "Announce Precence request received in commander." << std::endl;
			break;
		  }

		   case FOLLOW_REQUEST: {
			FollowRequest frq = cluon::extractMessage<FollowRequest>(std::move(envelope));
			std::cout << "Follow-Request received in commander." << std::endl;
			break;
		  }

		  case FOLLOW_RESPONSE: {
			FollowResponse frp = cluon::extractMessage<FollowRequest>(std::move(envelope));
			std::cout << "Follow-Response received in commander." << std::endl;
			break;
 		  }

		  case STOP_FOLLOW: {
			StopFollow stf = cluon::extractMessage<StopFollow>(std::move(envelope));
			std::cout << "Stop-Follow received in commander." << std::endl;
			break;
		  }

		  case FOLLOWER_STATUS: {
			FollowerStatus fls = cluon::extractMessage<FollowerStatus>(std::move(envelope));
			std::cout << "Follower-Status request received in commander." << std::endl;
			break;
		  }

		  case LEADER_STATUS: {
			LeaderStatus lds = cluon::extractMessage<LeaderStatus>(std::move(envelope));
			std::cout << "Leader-Status received in commander." << std::endl;
			break;
		  }

                  default: std::cout << "No matching case, wrong message type!" << std::endl;
              }
    });

}

//Testing methods. Used to test od4 sending and receiving.
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

