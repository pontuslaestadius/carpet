/*
  Author: Sebastian Fransson
  Created: 4/4 - 2018
  Last Updated: 12/4 - 2018

  Inspired by V2VProtocol: https://github.com/DIT168-V2V-responsibles/v2v-protocol
*/
#include "commander.hpp"

int main(){
	
	std::shared_ptr<commander> commanderService = std::make_shared<commander>();
	const int delay = 2000; // 2 second delay.
	std::cout << "Starting service.." << std::endl;
	// Wait so that we know that the service started properly and that the OD4 has been initialized.
	std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	std::cout << "Service running!" << std::endl;
	
	// Service should run until forced to shut down.
	while(1){
	std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	
	// Enter the chosen set of movements. OBS: This is for testing.
	 int choice;
	 std::this_thread::sleep_for(std::chrono::milliseconds(delay)); // Delay to give the program a chance to print the OD4 outcome.
       	 std::cout << "(1) testMove" << std::endl;
         std::cout << "(2) testTurnLeft" << std::endl;
         std::cout << "(3) testTurnRight" << std::endl;
	 std::cout << "(4) testStop" << std::endl;
       	 std::cout << "(5) testAnnounce" << std::endl;
         std::cout << "(6) testF-Req" << std::endl;
         std::cout << "(7) testF-Res" << std::endl;
	 std::cout << "(8) testF-Stop" << std::endl;
         std::cout << "(9) testF-Stat" << std::endl;
	 std::cout << "(10) testL-Stat" << std::endl;
	 std::cin >> choice;
	 switch(choice){
	   // Cases regarding speed and turning angle.
	   case 1:{ commanderService->testMove(); break;
	   }
	   case 2:{ commanderService->testTurnLeft(); break;
	   }
	   case 3:{ commanderService->testTurnRight(); break;
	   }
	   case 4:{ commanderService->testStop(); break;
	   }
	   
           // Cases for testing the V2V responses...
	   case 5:{ 
		commanderService->testAP(); break;
	   }
	   case 6:{  break;
	   }
	   case 7:{  break;
	   }
	   case 8:{  break;
	   }
	   case 9:{  break;
	   }
	   case 10:{  break;
	   }
	   default: ; // Empty since ghost inputs apparently happen from time to time.
	}
	

    }
	
}


// Control car through OD4 messages received etc.
commander::commander(){	
    // Connects to od4 session with CID:170 (Can be changed in the header file, OBS: group6 range 170-179).
    receivedMessage =
        std::make_shared<cluon::OD4Session>(CID,
          [this](cluon::data::Envelope &&envelope) noexcept {
		std::cout << "OD4 Session " << std::endl;

	      // Set up response depending on the message type received through the od4 session.
              switch (envelope.dataType()) {
                   case TURN_DIRECTION: { // Remember to check at what angle it wants to turn.
			// Unpacks the envelope and extracts the contained message. (Should potentially identify steering and move commands automatically).
                        Turn trn = cluon::extractMessage<Turn>(std::move(envelope)); // Should be enough??
                        std::cout << "received 'TURN' with angle  " << trn.steeringAngle() << " from controller'" << std::endl; 
			opendlv::proxy::GroundSteeringReading msgSteering;
		        msgSteering.steeringAngle(trn.steeringAngle()); // Turn appropriately, For forwarding to follower;
			receivedMessage->send(msgSteering);
			std::cout << "'TURN' message sent to car with angle " << trn.steeringAngle() << std::endl;
			forwardedMessage->send(trn);
		        std::cout << "'TURN' message with angle " << trn.steeringAngle() << " sent to v2v" << std::endl;
                        break;
		   }

		   case MOVE_FORWARD: {
			Move mo = cluon::extractMessage<Move>(std::move(envelope));
			std::cout << "received 'MOVE' from controller with speed  " << mo.percent() << std::endl;
			opendlv::proxy::PedalPositionReading msgPedal;
			msgPedal.percent(mo.percent()); // Move forward. For forwarding to follower.
			receivedMessage->send(msgPedal); //Sends a command to the motor telling it to move.
			std::cout << "'MOVE' message sent to car with speed " << mo.percent() << std::endl;
			forwardedMessage->send(mo);
			std::cout << "'MOVE' message with speed " << msgPedal.percent() <<  " sent to v2v" << std::endl;
		      	break;
                   }

		   case DISTANCE_READ: { //TODO: Incorporate this for follower mode.
			opendlv::proxy::DistanceReading dist = cluon::extractMessage<opendlv::proxy::DistanceReading>(std::move(envelope));
			std::cout << "received 'DISTANCE' from ultrasonic with distance  " << dist.distance() << std::endl;
		      	break;
                   }

 	  	   case FOLLOW_REQUEST: {
			FollowRequest frq = cluon::extractMessage<FollowRequest>(std::move(envelope));
			std::cout << "Follow-Request received in commander." << std::endl;
			break;
		   }

		   case 1041: {
			opendlv::proxy::PedalPositionReading steer = cluon::extractMessage<opendlv::proxy::PedalPositionReading>(std::move(envelope));
			break;
		   }
		   case 1045: {
			opendlv::proxy::GroundSteeringReading moverPer = cluon::extractMessage<opendlv::proxy::GroundSteeringReading>(std::move(envelope));
			break;
		   }

		  // In the case we recieve rogue messages.
                  default: std::cout << "No matching case for " << envelope.dataType() << ", wrong message type!" << std::endl;
              }
    });


/* 
	Opens a link to the OD4 link which is connected to the V2V protocol.
	Test receives in place to see that stuff works.
	Connects to od4 session with CID:171 (Can be changed in the header file, OBS: group6 range 170-179).				    
*/
    forwardedMessage =
        std::make_shared<cluon::OD4Session>(forwardCID,
          [this](cluon::data::Envelope &&envelope) noexcept {

	/*switch(envelope.dataType()){

	   //TODO: Create follower reactions.... Used for testing sending as of now.
	   case FORWARDED_MOVE: {
		Move forwardedMove = cluon::extractMessage<Move>(std::move(envelope));
	   	std::cout << "received a leader 'MOVE' instruction with speed " << forwardedMove.percent() << std::endl;
		break;
	   }

	   case FORWARDED_TURN: {
		Turn forwardedTurn = cluon::extractMessage<Turn>(std::move(envelope));
	   	std::cout << "received a leader 'TURN' instruction with angle " << forwardedTurn.steeringAngle() << std::endl;
		break;
	   }

	   case FOLLOW_RESPONSE: {
		FollowResponse frp = cluon::extractMessage<FollowResponse>(std::move(envelope));
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
	}*/
	  
    });

}

/*
	Testing methods. 
	Used to test od4 sending and receiving.
*/


void commander::testMove(){
	Move testMove;
	testMove.percent(0.25);
	receivedMessage->send(testMove);
}

void commander::testTurnLeft(){
	Turn testTurnLeft;
	testTurnLeft.steeringAngle(15.0);
	receivedMessage->send(testTurnLeft);
}

void commander::testTurnRight(){
	Turn testTurnRight;
	testTurnRight.steeringAngle(-15.0);
	receivedMessage->send(testTurnRight);
}

void commander::testStop(){
	Move testStop;
	testStop.percent(0.0);
	receivedMessage->send(testStop);
}
